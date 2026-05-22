import numpy as np
import math
from collections import defaultdict
import random
import plotly.graph_objects as go
import MDAnalysis as mda

l_mean = 3.8

class Protein:
    def __init__(self, sequence):
        if sequence is not None:
            self.sequence = sequence # 氨基酸序列，格式化为首字母大写
            self.length = len(sequence)
            self.generate_characteristics() # 得到氨基酸属性矩阵
            self.bonds = np.zeros((self.length-1, 3), dtype=float)      # 肽键信息（极坐标表示）(键长l, x轴夹角phi, 水平夹角theta)
            self.positions = np.zeros((self.length, 3), dtype=float)       # 残基位置，编号1残基位置为(0,0,0)
        else:
            pass

    def random_generate(self, length):
        '''
        随机生成氨基酸序列并初始化位置
        '''
        self.length = length
        
        all_Amino = ["Ala","Arg","Asn","Asp","Cys","Gln","Glu","Gly","His","Ile","Leu","Lys","Met","Phe","Pro","Ser","Thr","Trp","Tyr","Val"]
        random_list = [random.randint(0, 19) for _ in range(length)]
        self.sequence = [all_Amino[i] for i in random_list]
        
        self.bonds = np.zeros((self.length-1, 3), dtype=float)      # 肽键信息（极坐标表示）(键长l, x轴夹角phi, 水平夹角theta)
        self.positions = np.zeros((self.length, 3), dtype=float)       # 残基位置，编号1残基位置为(0,0,0)

        self.generate_characteristics() # 得到氨基酸属性矩阵

        self.initial_pos()

    def readpdb_generate(self, pdb_file_name, final=False):
        '''
        读取pdb文件，得到氨基酸序列、长度，
        final = True时，得到最终构象的氨基酸位置和肽键向量
        '''
        u = mda.Universe(pdb_file_name)
        # 生成氨基酸序列
        self.sequence = [
            residue.resname.strip().capitalize()  # 格式化为首字母大写（如Ala）
            for residue in u.residues
        ]
        self.length = len(self.sequence)
        print(f'蛋白质氨基酸链长度为{self.length}')
        self.bonds = np.zeros((self.length-1, 3), dtype=float)      # 肽键信息（极坐标表示）(键长l, x轴夹角phi, 水平夹角theta)
        self.positions = np.zeros((self.length, 3), dtype=float)       # 残基位置，编号1残基位置为(0,0,0)

        if final:
            # 得到最终构象的氨基酸位置和肽键向量
            calphas = u.select_atoms("protein and name CA")
            self.positions = calphas.positions
            for i in range(1, self.length):
                self.positions[i,:] -= self.positions[0,:]
            self.positions[0,:] = [0,0,0]
            self.pos2bond()

        self.generate_characteristics() # 得到氨基酸属性矩阵

    def initial_pos(self):
        '''
        初始化残基位置为x轴上的直线
        '''
        self.bonds[:,0] = np.full(self.length - 1, l_mean)
        self.bonds[:,1] = np.full(self.length - 1, 0)
        self.bonds[:,2] = np.full(self.length - 1, 0)
        self.bond2pos()

    def bond2pos(self):
        '''
        将肽键向量的极坐标表示转化为氨基酸的坐标表示
        '''
        x = 0
        y = 0
        z = 0
        for i in range(self.length-1):
            [l, phi, theta] = self.bonds[i,:]
            x += l * math.cos(phi) * math.sin(theta)
            y += l * math.sin(phi) * math.sin(theta)
            z += l * math.cos(theta)
            self.positions[i+1,:] = [x, y, z]

    def pos2bond(self):
        '''
        将氨基酸的坐标表示转换为肽键向量的极坐标表示
        '''
        for i in range(self.length-1):
            [dx, dy, dz] = self.positions[i+1,:] - self.positions[i,:]
            l = math.sqrt(dx**2 + dy**2 + dz**2)
            theta = math.acos(dz/l)
            d = math.sqrt(dx**2 + dy**2)
            if dy > 0:
                phi = math.acos(dx/d)
            else:
                phi = 2*math.pi - math.acos(dx/d)
            self.bonds[i,:] = [l, phi, theta]

    def generate_characteristics(self):
        ''' 
        生成amino的性质数组
        ''' 

        # 字典1：相对分子质量（四舍五入为整数）
        molecular_weight = {
            "Ala": 89, "Arg": 174, "Asn": 132, "Asp": 133,
            "Cys": 121, "Gln": 146, "Glu": 147, "Gly": 75,
            "His": 155, "Ile": 131, "Leu": 131, "Lys": 146,
            "Met": 149, "Phe": 165, "Pro": 115, "Ser": 105,
            "Thr": 119, "Trp": 204, "Tyr": 181, "Val": 117
        }

        # 字典2：亲疏水性（正值表示疏水性，负值表示亲水性）
        hydrophobicity = {
        'Ile': 4.5, 'Val':4.2, 'Leu':3.8, 'Phe':2.8, 'Cys':2.5, 'Met':1.9,
        'Ala':1.8, 'Gly':-0.4, 'Thr':-0.7, 'Ser':-0.8, 'Trp':-0.9, 'Tyr':-1.3,
        'Pro':-1.6, 'His':-3.2, 'Asn':-3.5, 'Asp':-3.5, 'Gln':-3.5, 'Glu':-3.5,
        'Lys':-3.9, 'Arg':-4.5
        }

        # 字典3：电荷性质（-1=负电，0=中性，1=正电）
        charge = {
            "Ala": 0, "Arg": 1, "Asn": 0, "Asp": -1,
            "Cys": 0, "Gln": 0, "Glu": -1, "Gly": 0,
            "His": 1, "Ile": 0, "Leu": 0, "Lys": 1,
            "Met": 0, "Phe": 0, "Pro": 0, "Ser": 0,
            "Thr": 0, "Trp": 0, "Tyr": 0, "Val": 0
        }

        # 字典4：特殊基团（0=无，1=羟基/酰胺基）
        special_groups = {
            "Ala": 0, "Arg": 0, "Asn": 2, "Asp": 0,
            "Cys": 0, "Gln": 2, "Glu": 0, "Gly": 0,
            "His": 0, "Ile": 0, "Leu": 0, "Lys": 0,
            "Met": 0, "Phe": 0, "Pro": 0, "Ser": 1,
            "Thr": 1, "Trp": 0, "Tyr": 1, "Val": 0
        }

        characteristic_dict = {0:molecular_weight,1:hydrophobicity,2:charge,3:special_groups}

        self.amino_characteristics = np.zeros((self.length, 4), dtype=int)
        for i in range(self.length):
            for j in range(4):
                self.amino_characteristics[i,j] = characteristic_dict[j][self.sequence[i]]

    def visualization(self):

        coords = self.positions
        sequence = self.sequence  # 氨基酸序列

        # 氨基酸颜色映射字典
        amino_color_dict = {
            # 非极性疏水氨基酸（红色系）
            'Ala': '#FF6666',  # 亮红（丙氨酸）
            'Val': '#FF4444',  # 朱红（缬氨酸）
            'Leu': '#FF0000',  # 纯红（亮氨酸）
            'Ile': '#CC0000',  # 深红（异亮氨酸）
            'Phe': '#FFB266',  # 橙红（苯丙氨酸）
            'Pro': '#FF8080',  # 粉红（脯氨酸）
            'Met': '#FF9933',  # 橙黄（甲硫氨酸）
            'Trp': '#E64D00',  # 棕红（色氨酸）
            
            # 酸性氨基酸（蓝色系）
            'Asp': '#0066FF',  # 天蓝（天冬氨酸）
            'Glu': '#3399FF',  # 湖蓝（谷氨酸）
            
            # 碱性氨基酸（品红色系）
            'Lys': '#FF00FF',  # 品红（赖氨酸）
            'Arg': '#CC00CC',  # 紫红（精氨酸）
            'His': '#FF66FF',  # 粉紫（组氨酸）
            
            # 极性中性氨基酸（绿色系）
            'Ser': '#00FF00',  # 翠绿（丝氨酸）
            'Thr': '#33CC33',  # 草绿（苏氨酸）
            'Cys': '#66FF66',  # 浅绿（半胱氨酸）
            'Asn': '#009933',  # 墨绿（天冬酰胺）
            'Gln': '#66CC00',  # 橄榄绿（谷氨酰胺）
            'Tyr': '#00CC99',  # 蓝绿（酪氨酸）
            
            # 特殊类型
            'Gly': '#A0A0A0',  # 灰色（甘氨酸）
        }

        # 构建轨迹线
        trace = go.Scatter3d(
            x=coords[:,0], y=coords[:,1], z=coords[:,2],
            mode='lines+markers',
            line=dict(width=4, color='rgb(150,150,150)'),
            marker=dict(
                size=5,
                color=[amino_color_dict[aa] for aa in sequence],  # 自定义颜色映射
                colorscale='Viridis',
                opacity=0.8
            )
        )

        fig = go.Figure(data=[trace])
        fig.update_layout(scene=dict(
            xaxis_title='X', yaxis_title='Y', zaxis_title='Z',
            aspectmode='data'  # 保持坐标比例真实
        ))
        fig.show()

    def save_to_txt(self):
        '''
        将蛋白质序列、构象坐标信息写入txt文件
        '''
        np.savetxt('positions.txt', self.positions)
        with open('sequence.txt', 'w') as f:
            f.write(self.sequence)
