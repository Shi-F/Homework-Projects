import torch
import torch.utils.data as data
import matplotlib.pyplot as plt
import numpy as np
from cleanlab.filter import find_label_issues
from InsectClassifier import InsectDataset, FNN, Trainer

# 配置参数
config = {
    "batch_size": 32,
    "learning_rate": 0.001,
    "num_epochs": 100,
    "device": torch.device("cuda" if torch.cuda.is_available() else "cpu")
}

class Cleaner:
    """
    基于Cleanlab库处理多分类数据集中的噪声标签
    """
    def __init__(self):
        pass

    def pretrain(self, dataset:InsectDataset):
        """
        在数据集上预训练一个FNN模型
        """
        # 创建数据加载器
        train_loader = data.DataLoader(dataset, batch_size=config["batch_size"], shuffle=True)
        # 初始化模型
        model = FNN(input_dim=dataset.X.shape[1])
        # 训练器
        trainer = Trainer(model, train_loader, None, None, device=config["device"], lr=config["learning_rate"],epochs=config["num_epochs"])
        # 训练模型
        trainer.train(verbose=False)
        self.model = trainer.model

    def find_issues(self, dataset:InsectDataset, show=False):
        """
        利用Cleanlab库的方法找到可能有问题的标签，并可视化结果（可选）
        """
        self.model.eval()
        with torch.no_grad():
            X_train = torch.tensor(dataset.X)
            y_train = dataset.y
            prob = torch.softmax(self.model(X_train), dim=1).numpy()
        issues = find_label_issues(
            labels=y_train,
            pred_probs=prob,
            frac_noise=2.0
        )

        # 输出结果
        print(f"find {issues.sum()} probable issues!")
        if show:
            self.show_issues(dataset=dataset, issues=issues)
        return issues
    
    def show_issues(self, dataset:InsectDataset, issues):
        """
        绘制数据散点图
        """
        points = {0: {"x":[], "y":[]}, 1: {"x":[], "y":[]}, 2: {"x":[], "y":[]}, "issue":{"x":[], "y":[]}}
        for i in range(dataset.len):
            row = dataset.xy[i]
            if not issues[i]:
                points[int(row[2])]["x"].append(row[0])
                points[int(row[2])]["y"].append(row[1])
            else:
                points["issue"]["x"].append(row[0])
                points["issue"]["y"].append(row[1])
        
        plt.figure(figsize=(8, 5))
        plt.scatter(points[0]["x"], points[0]["y"], color="blue", label="type0", s=10)
        plt.scatter(points[1]["x"], points[1]["y"], color="red", label="type1", s=10)
        plt.scatter(points[2]["x"], points[2]["y"], color="green", label="type2", s=10)
        plt.scatter(points["issue"]["x"], points["issue"]["y"], color="black", label="issues", marker="x",s=20)
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title("Data Points")
        plt.legend()
        plt.grid(True)  