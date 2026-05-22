from protein import Protein
from RRTStar import Node, RRTStar

if __name__ == "__main__":
    fname = input('请输入蛋白质结构文件名:')
    goal = Protein(None)
    goal.readpdb_generate(fname,final=True)
    goalnode = Node(goal.sequence, goal.bonds)
    start = Protein(None)
    start.readpdb_generate(fname,final=False)
    start.initial_pos()
    startnode = Node(start.sequence, start.bonds)

    RRTSolver = RRTStar(start=startnode, goal=goalnode, step_size=0.01, neighbor_radius=10*startnode.length**2, max_iter=10000, leading_weight=0.05)
    path = RRTSolver.plan()

    if path:
        print(f'规划完成, 路径长度为:{len(path)}')
    else:
        print('未找到路径！')

    RRTSolver.animation()