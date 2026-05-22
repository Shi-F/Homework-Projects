import math
import random
import numpy as np
from protein import Protein
from animation import ProteinVisualizer
from energy import EnergyCalculator

# 参数
l_mean = 3.8 # 平均键长
l_std = 0.4 # 键长极差的一半
weights = [1,10,10000,1000] # 能量的权重（基于数量级推算）
loose_rate = 0.05 # 能量松弛系数
p_rand = 0.1 # 随机取样时取到终点的概率

class Node(Protein):
    def __init__(self, sequence:list, bonds:np.ndarray):
        super().__init__(sequence)
        self.bonds = bonds    # 肽键信息
        self.bond2pos()     # 计算氨基酸位置
        self.parent = None  # 父节点
        self.cost = 0.0     # 从根节点到本节点的路径代价

class RRTStar:
    def __init__(self, start:Node, goal:Node, step_size:float, neighbor_radius:float, max_iter=10000, leading_weight=0):
        '''
        参数：
        start:起始节点，goal:终止节点，step_size：步长，neighbor_radius:搜索近邻节点的半径，max_iter:最大迭代次数，leading_weight:取样时终点的权重
        '''
        self.start = start  # 起始点
        self.goal = goal      # 目标点
        self.protein_len = start.length
        self.step_size = step_size              # 扩展步长
        self.neighbor_radius = neighbor_radius  # 邻域搜索半径
        self.max_iter = max_iter                # 最大迭代次数
        self.leading_weight = leading_weight    # 取样时终点所占权重
        self.tree = [self.start]                # 初始化树
        self.move_distance = 0                  # 每步移动距离的均值
        self.energy_calculator = EnergyCalculator()
        self.energy_calculator.calculate_final_angles(final_positions = self.goal.positions)

        n = start.length
        self.bonds_delta = np.zeros((n-1, 3), dtype=float)
        self.bonds_delta[:,0] = np.full(n-1, l_std*self.step_size)
        self.bonds_delta[:,1] = np.full(n-1, math.pi*2*self.step_size)
        self.bonds_delta[:,2] = np.full(n-1, math.pi*self.step_size)

        self.energy_loose = (self.calculate_energy(self.start) - self.calculate_energy(self.goal))*loose_rate

    def plan(self):
        '''
        规划起点到终点的路径
        '''
        min_dist = math.inf
        tree_edge_num = 0 # 树的边数
        for i in range(self.max_iter):
            # 1. 随机采样（有一定概率采样到目标点）
            if random.random() > p_rand:
                rand_node = self.sample()
            else:
                rand_node = self.goal

            # 2. 寻找最近邻节点
            nearest_node = self.nearest_node(rand_node)

            # 3. 向采样点方向扩展新节点
            new_node = self.steer(nearest_node, rand_node)
            new_node.parent = nearest_node # 暂时记录父节点

            # 4. 碰撞检测
            if self.collision_free(nearest_node, new_node):
                # 5. 寻找邻近节点集
                near_nodes = self.find_nearby_nodes(new_node)

                # 6. 选择最优父节点
                self.choose_parent(new_node, near_nodes)

                # 7. 添加到树中
                self.tree.append(new_node)
                tree_edge_num += 1
                self.move_distance = (self.move_distance * (tree_edge_num-1) + self.distance(new_node,new_node.parent))/tree_edge_num # 更新树相邻节点的平均距离

                # 8. 重新布线
                self.rewire(new_node, near_nodes, nearest_node)

                # 9. 检查是否到达目标
                if self.reached_goal(new_node):
                    return self.generate_path(new_node)
            
            new_dist = self.distance(new_node, self.goal)
            if min_dist > new_dist:
                min_dist = new_dist
                print(f"iteration:{i},min_distance:{min_dist:.4e}")
            if i % 500 == 0:
                print(f"iteration:{i}")

        return None  # 未找到路径

    def calculate_energy(self, node:Node):
        '''
        计算过程中蛋白质构象的能量
        '''
        self.energy_calculator.Process_Protein(node)
        return self.energy_calculator.calculate_energy(weights=weights)

    def animation(self):
        '''
        展示折叠路径的3D动画
        '''
        coords = [pro.positions for pro in self.path]
        visualizer = ProteinVisualizer(coords, self.start.sequence, self.energy)
        fig = visualizer.create_animation(frame_duration=100)
        fig.show()

    def sample(self):
        '''
        在可能的空间内随机采样，再与终点位置加权平均
        '''
        n = len(self.start.sequence)
        new_bonds = np.zeros((n-1, 3))
        for i in range(n-1):
            l = random.uniform(l_mean - l_std, l_mean + l_std)
            phi = random.uniform(0, 2*math.pi)
            if phi == 2*math.pi:
                phi = 0
            theta = random.uniform(0, math.pi)
            new_bonds[i,:] = np.array([l, phi, theta])*(1-self.leading_weight) + self.goal.bonds[i,:]*self.leading_weight # 加权平均，向终点方向引导

        new_node = Node(self.start.sequence, new_bonds)
        new_node.bond2pos()  

        return new_node

    def nearest_node(self, rand_node):
        """寻找最近邻节点"""
        return min(self.tree, key=lambda node: self.distance(node, rand_node))

    def steer(self, from_node:Node, to_node:Node):
        """生成新节点（按步长扩展）"""  
        new_bonds = from_node.bonds + self.bonds_delta * compare_bonds(from_node, to_node)
        
        new_node = Node(from_node.sequence, new_bonds)
        new_node.bond2pos()

        new_dist = self.distance(from_node, new_node)
        dist = self.distance(from_node, to_node)
        if dist <= new_dist:
            return to_node
        else:
            return new_node
            
    def distance(self, node1:Node, node2:Node):
        '''
        计算2个节点的位置每对欧氏距离的平方和
        '''
        pos1 = node1.positions
        pos2 = node2.positions
        return ((pos1 - pos2)**2).sum()

    def collision_free(self, from_node:Node, to_node:Node):
        """碰撞检测：检测下一个构象的能量是否低于当前构象"""
        return self.calculate_energy(from_node) - self.calculate_energy(to_node) >= -self.energy_loose

    def find_nearby_nodes(self, new_node):
        """寻找邻近节点"""
        return [node for node in self.tree if self.distance(node, new_node) <= self.neighbor_radius]

    def choose_parent(self, new_node:Node, near_nodes:list[Node]):
        """选择最优父节点"""
        min_cost = math.inf
        for node in near_nodes:
            # 计算通过该节点到新节点的cost
            # 如果更优则更新父节点
            new_cost = node.cost + self.distance(new_node, node)
            if new_cost < min_cost:
                new_node.cost = new_cost
                new_node.parent = node

    def rewire(self, new_node:Node, near_nodes:list[Node], last_parent:Node):
        """重新布线邻近节点"""
        for node in near_nodes:
            # 检查是否可以通过新节点获得更优路径
            # 如果可行则更新父节点
            if node != new_node.parent:
                # 更新非当前父节点的父节点
                new_cost = new_node.cost + self.distance(new_node, node)
                if new_cost < node.cost:
                    node.parent = new_node
                    node.cost = new_cost
            else:
                # 更新当前父节点的父节点
                new_cost = last_parent.cost + self.distance(last_parent, new_node) + self.distance(new_node, node)
                if new_cost < node.cost:
                    node.parent = new_node
                    new_node.parent = last_parent
                    node.cost = new_cost
                    new_node.cost = last_parent.cost + self.distance(last_parent, new_node)

    def reached_goal(self, node:Node):
        """检查是否到达目标区域"""
        return self.distance(node, self.goal) <= self.move_distance

    def generate_path(self, final_node:Node):
        """回溯生成路径"""
        path = [self.goal]
        energy_path = [self.calculate_energy(self.goal)]
        current = final_node
        while current:
            path.append(current) # 记录状态
            energy_path.append(self.calculate_energy(current)) # 记录能量变化
            current = current.parent         
        self.path = path[::-1]
        self.energy = energy_path[::-1]
        return self.path.copy()  # 反转路径

def compare_bonds(node1:Node, node2:Node):
    '''
    比较两个结点肽键矢量的大小，返回1,-1组成的矩阵
    '''
    comparison = np.array((node2.bonds - node1.bonds) >= 0, dtype=int)
    comparison[comparison == 0] = -1
    return comparison
