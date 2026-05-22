import torch
import torch.utils.data as data
import matplotlib.pyplot as plt
import numpy as np
from InsectClassifier import FNN, InsectDataset, Trainer
from KMeansForInsectClassifier import denoise_with_kmeans
import time

# 配置参数，可能需要手动修改绝对路径
config = {
    "train_path": "data/insects-2-training.txt",
    "test_path_fromtrain": "data/insects-2-testing-fromtrain.txt",
    "test_path_new": "data/insects-2-testing-new.txt",   
    "batch_size": 32,
    "input_dim": 2,
    "hidden_size": 64,
    "num_classes": 3,
    "learning_rate": 0.001,
    "num_epochs": 100,
    "loss_print_frequency": 10,
    "threshold_multiplier":1.4, 
    "device": torch.device("cuda" if torch.cuda.is_available() else "cpu")
}

class KMeansInsectDataset(InsectDataset):
    """
    自定义数据集类，加载数据为 Pytorch 数据集，可基于KMeans聚类的方法进行噪声过滤
    """
    def clean_self(self, show=False):
        """
        清理数据集内的噪声，并可视化结果（可选）
        """
        denoised_X, denoised_y = denoise_with_kmeans(self.X, self.y, k_per_class=3, threshold_multiplier=config["threshold_multiplier"])

        denoised_xy = np.hstack((denoised_X, denoised_y.reshape((denoised_y.shape[0],1))))
        
        if show:
            issues = ~np.isin(self.xy, denoised_xy).all(axis=1)
            self.show_issues(self, issues)

            # 输出去噪准确率
            origin_dataset = InsectDataset(r"F:\2024-2025春季学期\数学建模\第三次作业\insects-training.txt")
            mistakes = 0
            correct = 0

            noisey = self.xy[:,2]
            originy = origin_dataset.xy[:,2]

            for i in range(self.X.shape[0]):
                if noisey[i] != originy[i]:
                    mistakes = mistakes + 1
                    if issues[i]:
                        correct = correct + 1
            print(f"True issues:{mistakes}, Found issues:{correct}, Accuracy:{correct/mistakes}")
        
        self.X = denoised_X
        self.y = denoised_y
        self.len = denoised_X.shape[0]
        self.xy = np.hstack((self.X, self.y.reshape((self.len,1))))

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
        
    
# 主程序
if __name__ == "__main__":
    # 创建数据集
    train_dataset = KMeansInsectDataset(config["train_path"])
    test_fromtrain_dataset = KMeansInsectDataset(config["test_path_fromtrain"])
    test_new_dataset = KMeansInsectDataset(config["test_path_new"])
 
 
    # 绘制数据散点图
    train_dataset.plot_points()
    test_fromtrain_dataset.plot_points()
    test_new_dataset.plot_points()

    # 清理训练数据集内噪声
    issues = train_dataset.clean_self(show=True)

    # 创建数据加载器
    train_loader = data.DataLoader(train_dataset, batch_size=config["batch_size"], shuffle=True)
    test_loader_fromtrain = data.DataLoader(test_fromtrain_dataset, batch_size=config["batch_size"], shuffle=False)
    test_loader_new = data.DataLoader(test_new_dataset, batch_size=config["batch_size"], shuffle=False)

    # 初始化模型
    model = FNN(input_dim=config["input_dim"])

    # 训练器
    trainer = Trainer(model, train_loader, test_loader_fromtrain, test_loader_new, device=config["device"], lr=config["learning_rate"],epochs=config["num_epochs"])

    # 训练模型
    start_time = time.time() # 记录起始时间
    trainer.train()
    end_time = time.time()
    print(f"Running time: {end_time - start_time:.2f}s.")

    # 评估模型
    trainer.final_performance()

    # 绘制误差曲线和准确率曲线
    trainer.plot_losses_and_accuracy()

    # 绘制训练与测试结果散点图
    trainer.plot_result()

    plt.show()