import random
import math
import numpy as np
from animation import ProteinVisualizer

# 生成方向选择表
const_dir = [(1,0,0), (-1,0,0), (0,1,0), (0,-1,0), (0,0,1), (0,0,-1)]

def calculate_energy(positions:np.ndarray, sequence):
    """计算当前构型的能量（H-H非相邻接触数的负值）"""
    # 创建位置->索引字典
    pos_dict = {}
    for i in range(positions.shape[0]):
        pos = tuple(positions[i,:].tolist())
        pos_dict[pos] = i
    
    # 计算能量
    contact_count = 0
    for pos, i in pos_dict.items():
        if sequence[i] != 'H':
            continue
        
        x, y, z = pos
        # 检查所有六个相邻方向
        for dx, dy, dz in const_dir:
            neighbor = (x+dx, y+dy, z+dz)
            if neighbor in pos_dict:
                j = pos_dict[neighbor]
                # 确保不是序列中的直接邻居
                if abs(j - i) != 1 and sequence[j] == 'H':
                    contact_count += 1
    
    # 每个接触被计算两次，所以需要除以2
    return -contact_count // 2

def generate_new_solution(positions:np.ndarray): 
    """生成新的折叠方案"""
    current_positions = positions.tolist()
    n = len(current_positions)
    if n <= 1:
        return current_positions.copy()
    
    # 随机选择转折点
    i = random.randint(0, n-2)
    current_dir = (
        current_positions[i+1][0] - current_positions[i][0],
        current_positions[i+1][1] - current_positions[i][1],
        current_positions[i+1][2] - current_positions[i][2]
    )
    
    # 生成可能的新方向
    possible_dirs = []
    for dx, dy, dz in const_dir:
        if (dx, dy, dz) != current_dir and (dx, dy, dz) != (-current_dir[0], -current_dir[1], -current_dir[2]):
            possible_dirs.append((dx, dy, dz))
    
    if not possible_dirs:
        return current_positions.copy()
    
    new_dir = random.choice(possible_dirs)
    new_pos = [
        current_positions[i][0] + new_dir[0],
        current_positions[i][1] + new_dir[1],
        current_positions[i][2] + new_dir[2]
    ]
    
    # 检查碰撞
    if new_pos in current_positions:
        return current_positions.copy()
    
    # 构建新路径
    new_positions = current_positions.copy()
    new_positions[i+1] = new_pos
    current = new_pos
    existing = new_positions[:i+2]
    
    for j in range(i+2, n):
        next_pos = [
            current[0] + new_dir[0],
            current[1] + new_dir[1],
            current[2] + new_dir[2]
        ]
        if next_pos in existing:
            return current_positions.copy()
        existing.append(next_pos)
        new_positions[j] = next_pos
        current = next_pos
    return new_positions

def simulated_annealing(sequence:list, initial_positions:np.ndarray, T=10.0, T_min=1e-3, alpha=0.95, iterations=100):
    """模拟退火主算法"""
    current_sol = initial_positions.copy()
    current_energy = calculate_energy(current_sol, sequence)
    
    best_sol = current_sol.copy()
    best_energy = current_energy
    path = [best_sol]
    energy_history = [best_energy]
    form_best_energy = best_energy

    while T > T_min:

        for _ in range(iterations):
            new_sol = np.array(generate_new_solution(current_sol))
            new_energy = calculate_energy(new_sol, sequence)
            
            delta = new_energy - current_energy
            if delta < 0 or random.random() < math.exp(-delta / T): # Metropolis规则
                current_sol = new_sol.copy()
                current_energy = new_energy
                
                if current_energy < best_energy:
                    best_sol = current_sol.copy()
                    best_energy = current_energy
            
        T *= alpha
        if best_energy < form_best_energy:
            print(f"Temperature: {T:.4f}, Best Energy: {best_energy}") # 新构象能量降低时输出
            form_best_energy = best_energy
            path.append(best_sol)   # 保存路径上的构象
            energy_history.append(best_energy)

    return best_sol, best_energy, path, energy_history
    
def random_sequence(len):
    HPsequence = ''
    for _ in range(len):
        aa = random.choice(['H','P'])
        HPsequence += aa
    return HPsequence

def initial_positions(len):
    '''
    随机生成初始位置
    '''
    positions = np.zeros((len,3), dtype=int)
    for i in range(1,len):
        dir = const_dir.copy()
        flag = True
        while flag:
            flag = False
            delta = random.choice(dir)
            positions[i,:] = positions[i-1,:] + np.array(delta)
            for j in range(i):
                if (positions[i,:] == positions[j,:]).all():
                    dir.remove(delta)
                    flag = True
    return positions

if __name__ == "__main__":
    # 输入参数
    length = 40
    sequence = random_sequence(length)
    print(f'氨基酸序列为:{sequence}')
    
    # 运行模拟退火，进行多轮搜索避免陷入局部极小值
    best_energy = 0
    for i in range(10):
        print(f'第{i+1}次搜索:')
        positions = initial_positions(length)
        best_config, energy, path, energy_history = simulated_annealing(
            sequence=sequence,
            initial_positions=positions,
            T=100.0,
            T_min=1e-3,
            alpha=0.95,
            iterations=2000
        )
        if energy < best_energy:
            best_path = path
            best_energy_history = energy_history
            best_energy = energy

    # 可视化
    ani = ProteinVisualizer(best_path, sequence, best_energy_history, isHP=True)
    fig = ani.create_animation()
    fig.show()