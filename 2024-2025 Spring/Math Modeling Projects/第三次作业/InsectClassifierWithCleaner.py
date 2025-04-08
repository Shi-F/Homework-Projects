import torch
import torch.utils.data as data
import matplotlib.pyplot as plt
from InsectClassifier import FNN, InsectDataset, Trainer
from CleanlabForInsectClassifier import Cleaner
import time

# 配置参数，需要手动修改路径
config = {
    "train_path": r"F:\2024-2025春季学期\数学建模\第三次作业\insects-2-training.txt",
    "test_path_fromtrain": r"F:\2024-2025春季学期\数学建模\第三次作业\insects-2-testing-fromtrain.txt",
    "test_path_new": r"F:\2024-2025春季学期\数学建模\第三次作业\insects-2-testing-new.txt",   
    "batch_size": 32,
    "input_dim": 2,
    "hidden_size": 64,
    "num_classes": 3,
    "learning_rate": 0.001,
    "num_epochs": 100,
    "loss_print_frequency": 1,
    "device": torch.device("cuda" if torch.cuda.is_available() else "cpu")
}

class CleanerInsectDataset(InsectDataset):
    """
    自定义数据集类，加载数据为 Pytorch 数据集，可基于Cleanlab库的方法进行噪声过滤
    """
    def clean_self(self, cleaner:Cleaner):
        """
        清理数据集内的可疑噪声
        """
        issues = cleaner.find_issues(self,show=True)

        mask = ~issues
        self.X = self.X[mask]
        self.y = self.y[mask]
        self.xy = self.xy[mask]
        self.len = self.X.shape[0]
        print("Cleaning finished.")
    
# 主程序
if __name__ == "__main__":
    # 创建数据集
    train_dataset = CleanerInsectDataset(config["train_path"])
    test_fromtrain_dataset = CleanerInsectDataset(config["test_path_fromtrain"])
    test_new_dataset = CleanerInsectDataset(config["test_path_new"])
 
    # 绘制数据散点图
    train_dataset.plot_points()
    test_fromtrain_dataset.plot_points()
    test_new_dataset.plot_points()

    # 清理训练数据集内噪声
    cleaner = Cleaner()
    cleaner.pretrain(train_dataset)
    train_dataset.clean_self(cleaner=cleaner)


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