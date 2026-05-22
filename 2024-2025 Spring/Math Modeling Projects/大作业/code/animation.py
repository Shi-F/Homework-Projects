import plotly.graph_objects as go
from plotly.subplots import make_subplots
import numpy as np

class ProteinVisualizer:
    def __init__(self, positions_history, sequence, energy_history, isHP=False):
        """
        参数：
        positions_history: 各时间步的氨基酸坐标列表，每个元素是(N,3)的numpy数组
        sequence: 氨基酸序列（列表形式）
        """
        self.positions_history = positions_history
        self.sequence = sequence
        self.energy_history = energy_history

        # 验证数据一致性
        assert len(positions_history) == len(energy_history), "坐标数据与能量数据长度不一致"
        
        # 定义颜色映射
        if isHP:
            self.amino_color_dict = {'H':'#FF0000','P':'#0066FF'}
        else:
            self.amino_color_dict = {
                # 非极性疏水氨基酸（红色系）
                'Ala': '#FF6666', 'Val': '#FF4444', 'Leu': '#FF0000',
                'Ile': '#CC0000', 'Phe': '#FFB266', 'Pro': '#FF8080',
                'Met': '#FF9933', 'Trp': '#E64D00',
                # 酸性氨基酸（蓝色系）
                'Asp': '#0066FF', 'Glu': '#3399FF',
                # 碱性氨基酸（品红色系）
                'Lys': '#FF00FF', 'Arg': '#CC00CC', 'His': '#FF66FF',
                # 极性中性氨基酸（绿色系）
                'Ser': '#00FF00', 'Thr': '#33CC33', 'Cys': '#66FF66',
                'Asn': '#009933', 'Gln': '#66CC00', 'Tyr': '#00CC99',
                # 特殊类型
                'Gly': '#A0A0A0'
            }
        
        # 生成颜色列表
        self.color_list = [self.amino_color_dict[aa] for aa in self.sequence]
    
    def create_animation(self, frame_duration=500):
        """创建3D动画"""
        # 初始化图形
        fig = make_subplots(
            rows=1, cols=2,
            specs=[[{'type': 'scene'}, {'type': 'xy'}]],
            column_widths=[0.6, 0.4],
            horizontal_spacing=0.05
        )
        
        # 初始化3D结构可视化
        initial_coords = self.positions_history[0]
        fig.add_trace(go.Scatter3d(
            x=initial_coords[:,0], y=initial_coords[:,1], z=initial_coords[:,2],
            mode='lines+markers',
            line=dict(width=2, color='rgb(150,150,150)'),
            marker=dict(
                size=2,
                color=self.color_list,
                opacity=0.5
            ),
            name='Protein'
        ))

        # 初始化能量曲线
        fig.add_trace(
            go.Scatter(
                x=list(range(len(self.energy_history))),
                y=self.energy_history,
                mode='lines',
                name='Energy Profile',
                line=dict(color='royalblue', width=2)
            ),
            row=1, col=2
        )

        # 添加初始能量标记点
        fig.add_trace(
            go.Scatter(
                x=[0],
                y=[self.energy_history[0]],
                mode='markers',
                marker=dict(
                    color='red',
                    size=10,
                    line=dict(width=2, color='white')
                ),
                name='Current Frame'
            ),
            row=1, col=2
        )

        # 创建动画帧
        frames = []
        for i, coords in enumerate(self.positions_history):
            frames.append(go.Frame(
                data=[go.Scatter3d(
                    x=coords[:,0],
                    y=coords[:,1],
                    z=coords[:,2],
                    mode='lines+markers'),
                    go.Scatter(
                        x=[i],
                        y=[self.energy_history[i]],
                        mode='markers',
                        marker=dict(color='red', size=10)
                    )
                ],
                traces=[0, 2],  # 指定更新第0和第1个trace
                name=f'frame_{i}'
            ))

        fig.frames = frames

        # 设置动画控件
        fig.update_layout(
            updatemenus=[{
                "type": "buttons",
                "buttons": [
                    {
                        "label": "▶️ Play",
                        "method": "animate",
                        "args": [
                            None, 
                            {
                                "frame": {"duration": frame_duration, "redraw": True},
                                "fromcurrent": True,
                                "transition": {"duration": 0}
                            }
                        ]
                    },
                    {
                        "label": "⏸ Pause",
                        "method": "animate",
                        "args": [
                            [None],
                            {
                                "frame": {"duration": 0, "redraw": False},
                                "mode": "immediate"
                            }
                        ]
                    }
                ]
            }],
            sliders=[{
                "active": 0,
                "steps": [
                    {
                        "args": [[f.name], {"frame": {"duration": 0, "redraw": True}}],
                        "label": f"Step {i}",
                        "method": "animate"
                    }
                    for i, f in enumerate(fig.frames)
                ]
            }]
        )

        # 设置场景布局
        fig.update_layout(
            scene=dict(
                xaxis_title='X',
                yaxis_title='Y',
                zaxis_title='Z',
                aspectmode='data',
                camera=dict(eye=dict(x=1.5, y=1.5, z=1.5),center=dict(x=0, y=0, z=0))
            ),
            scene2=dict(  # 右侧子图设置
                xaxis=dict(title='Frame', showgrid=True),
                yaxis=dict(title='Energy', showgrid=True),
            ),
            title="Protein Folding Animation",
            width=1500,
            height=600
        )

        return fig
