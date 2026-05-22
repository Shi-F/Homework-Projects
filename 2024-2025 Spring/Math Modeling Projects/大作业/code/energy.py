
import numpy as np
import math
from scipy.spatial import KDTree
from protein import Protein

class EnergyCalculator:
    def Process_Protein(self,protein:Protein):
        self.amino_positions = protein.positions
        self.length = protein.length
        self.amino_charges = protein.amino_characteristics[:,2]
        self.sequence_hydrophobicity = protein.amino_characteristics[:,1].T
        self.dist_matrix = np.linalg.norm(self.amino_positions[:, None, :] - self.amino_positions[None, :, :], axis=-1)

    def calculate_charge_energy(self):
        ''' 
        计算电势能 单位kJ/mol
        '''
        Avogadro_constant = 6.02214076e23 # 阿伏伽德罗常数
        e = 1.6021766208e-19
        epsion = 8.854187817E-12
        multiple_matrix = self.amino_charges @ self.amino_charges.T
        with np.errstate(divide='ignore', invalid='ignore'):
            charge_matrix = np.divide(multiple_matrix,(self.dist_matrix*0.1)**2)  #计算氨基酸之间的电势能 需要将Å转化为m,1m = 10^10 Å
        return Avogadro_constant/(4*math.pi*epsion)*e**2/1e3*np.sum(np.triu(charge_matrix, k=1))


    def calculate_VDW_energy(self):
        ''' 
        计算范德华力势能 单位 kJ/mol
        '''
        B = 11.5E-6 #排斥项系数 (单位kJ*nm^12/mol)
        A = 5.96E-3 #吸引项系数 (单位kJ*nm^6/mol)

        dist_matrix = self.dist_matrix
        with np.errstate(divide='ignore', invalid='ignore'):
            VDW_matrix = B/dist_matrix**12-A/dist_matrix**6
        upper_triangle = np.triu(VDW_matrix, k=1)
        return np.sum(upper_triangle) 


    def calculate_hydrophobic_energy(self,radius=8.0, gamma=3.0):
        ''' 
        计算疏水势能 单位kJ/mol  疏水势能越大，蛋白质稳定性越高，因此返回值取负值
        radius 为局部半径
        sequence_hydrophobicity  为 Eisenberg标度 描述氨基酸序列疏水性
        γ 为k_B * T_e  单位为 ​kJ·Å³/mol
        '''
        tree = KDTree(self.amino_positions)
        densities = []
        for i in range(len(self.amino_positions)):
            neighbors = tree.query_ball_point(self.amino_positions[i], radius)
            n_beta = len(neighbors) - 1  # 排除自身
            volume = (4/3)*np.pi*(radius**3)
            densities.append(n_beta / volume)
        total_energy = gamma * self.sequence_hydrophobicity @ np.array(densities).T   # @为矩阵乘法
        return -total_energy

    def calculate_torsion_energy(self,actual_angles,ideal_angles_list,k=1):
        """
        计算二面角约束能量
        :param actual_angles: 实际二面角列表 [theta1,theta2...] 
        :param ideal_angles: 理想角度字典 
        :return: 二面角能量 (kJ/mol)
        """
        return k*np.sum(np.square(actual_angles - ideal_angles_list))


    def calculate_angle(self,amino_positions):
        ''' 
        计算二面角弧度 
        self.amino_positions: 氨基酸坐标
        '''
        vectors = amino_positions[1:,:] - amino_positions[0:-1,:]
        norms = np.sqrt(np.sum(np.square(vectors), axis=1))
        down = norms[0:-1]*norms[1:]
        up = np.sum(vectors[0:-1]*vectors[1:], axis=1)
        cos_thetas = np.clip(up/down, -1.0, 1.0)
        angle_arc = math.pi - np.arccos(cos_thetas) # 每个角的弧度值
        return angle_arc
    
    def calculate_final_angles(self,final_positions):
        self.final_amino_angles = self.calculate_angle(final_positions)

    def calculate_energy(self,weights):
        e1 = self.calculate_charge_energy()
        e2 = self.calculate_VDW_energy()
        e3 = self.calculate_hydrophobic_energy()
        e4 = self.calculate_torsion_energy(self.calculate_angle(self.amino_positions),self.final_amino_angles) 

        return np.sum(np.array(weights)*np.array([e1,e2,e3,e4]))

