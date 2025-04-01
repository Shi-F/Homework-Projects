import torch
import torch.nn as nn
import torch.optim as optim
import torch.utils.data as data
import matplotlib.pyplot as plt
import numpy as np
import time

# 配置参数
config = {
    "train_path": r"F:\2024-2025春季学期\数学建模\第三次作业\insects-2-training.txt",
    "test_path": r"F:\2024-2025春季学期\数学建模\第三次作业\insects-2-testing.txt",
    "batch_size": 32,
    "input_dim": 2,
    "hidden_size": [128, 64, 32],
    "num_layers": 3,
    "num_classes": 3,
    "learning_rate": 0.001,
    "num_epochs": 100,
    "loss_print_frequency": 1,
    "device": torch.device("cuda" if torch.cuda.is_available() else "cpu")
}

class InsectDataset(data.Dataset):
    """
    自定义数据集类，加载数据为 Pytorch 数据集
    """
    def __init__(self, filepath):
        # 从文本文件中读入数据
        self.xy = np.loadtxt(fname=filepath, delimiter=' ', dtype=np.float32, encoding='utf-8')
        self.len = self.xy.shape[0]

        # 特征与标签
        self.X = self.xy[:, :2].astype(np.float32)
        self.y = self.xy[:, [-1]].astype(np.int64).flatten()
        

    def __len__(self):
        # 返回数据集大小
        return self.len
    
    def __getitem__(self, index):
        return (
            torch.tensor(self.X[index]),
            torch.tensor(self.y[index])
        )
    
    def plot_points(self):
        """
        绘制数据散点图
        """
        points = {0: {"x":[], "y":[]}, 1: {"x":[], "y":[]}, 2: {"x":[], "y":[]}}
        for row in self.xy:
            points[int(row[2])]["x"].append(row[0])
            points[int(row[2])]["y"].append(row[1])
        
        plt.figure(figsize=(8, 5))
        plt.scatter(points[0]["x"], points[0]["y"], color="blue", label="type0", s=10)
        plt.scatter(points[1]["x"], points[1]["y"], color="red", label="type1", s=10)
        plt.scatter(points[2]["x"], points[2]["y"], color="green", label="type2", s=10)
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title("Data Points")
        plt.legend()
        plt.grid(True)   

            
    
class FNN(nn.Module):
    """
    经典的全连接神经网络（Feedforward Neural Network, FNN）
    结构：输入层 -> 隐藏层(ReLU) -> 输出层
    """
    def __init__(self, input_dim):
        """
        初始化结构：全连接的输入层+单个隐藏层+输出层
        """
        super(FNN, self).__init__()
        self.fc1 = nn.Linear(input_dim, config["hidden_size"][0]) # 输入层到隐藏层
        self.relu = nn.ReLU() # ReLU激活函数
        self.fc2 = nn.Linear(config["hidden_size"][0], config["hidden_size"][1]) # 隐藏层到第二隐藏层
        self.fc3 = nn.Linear(config["hidden_size"][1], config["hidden_size"][2]) # 第二隐藏层到第三隐藏层
        self.fc4 = nn.Linear(config["hidden_size"][2], config["num_classes"]) # 第三隐藏层到输出层

    def forward(self, x):
        """
        向前传播
        """
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.relu(x)
        x = self.fc3(x)
        x = self.relu(x)
        x = self.fc4(x)
        return x
    
class Trainer:
    """
    训练和评估神经网络模型的类
    """
    def __init__(self, model, train_loader, test_loader, device=config["device"], lr=config["learning_rate"], epochs=config["num_epochs"]):
        self.model = model.to(device)
        self.train_loader = train_loader
        self.test_loader = test_loader
        self.device = device
        self.epochs = epochs

        # 使用交叉熵损失作为损失函数
        self.criterion = nn.CrossEntropyLoss()

        # 使用Adam优化器
        self.optimizer = optim.Adam(self.model.parameters(), lr=lr)

        # 记录误差
        self.train_losses = []
        self.test_losses = []

        # 记录正确率
        self.train_accuracy = []
        self.test_accuracy = []

        # 记录最后一次训练和测试的结果
        self.train_result = {}
        self.test_result = {}

    def train(self):
        """
        训练模型
        """
        self.model.train()
        for epoch in range(self.epochs):
            total_loss = 0
            train_correct = 0
            train_total = 0
            for batch_X, batch_y in self.train_loader:
                batch_X, batch_y = batch_X.to(self.device), batch_y.to(self.device)
                self.optimizer.zero_grad() # 梯度清零
                outputs = self.model(batch_X) # 前向传播
                loss = self.criterion(outputs, batch_y) # 计算损失
                loss.backward() # 反向传播
                self.optimizer.step() # 更新参数

                total_loss += loss.item()

                _, predicted = torch.max(outputs, 1)
                train_correct += (predicted == batch_y).sum().item()
                train_total += len(batch_y)

                # 记录最后一次训练的结果
                if epoch == self.epochs - 1:
                    for i in range(len(batch_y)):
                        self.train_result[tuple(batch_X[i].tolist())] = (predicted[i] == batch_y[i]).item()

            # 计算训练误差
            avg_train_loss = total_loss / len(self.train_loader)
            self.train_losses.append(avg_train_loss)

            # 计算训练集上的准确率
            train_accuracy = train_correct / train_total * 100
            self.train_accuracy.append(train_accuracy)

            # 计算测试误差
            avg_test_loss = self.evaluate()
            self.test_losses.append(avg_test_loss)

            # 每轮打印一次损失
            if (epoch + 1) % config["loss_print_frequency"] == 0:
                print(f"Epoch [{epoch+1}/{self.epochs}], Train Loss: {avg_train_loss:.4f}, Test Loss: {avg_test_loss:.4f}, Train Accuracy: {train_accuracy:.2f}%, Test Accuracy: {self.test_accuracy[epoch]:.2f}%")

    def evaluate(self, final = False):
        """
        在测试集上评估模型
        """
        self.model.eval() # 设置为评估模式
        total_loss = 0
        test_correct = 0
        test_total = 0
        with torch.no_grad(): # 不计算梯度
            for batch_X, batch_y in self.test_loader:
                batch_X, batch_y = batch_X.to(self.device), batch_y.to(self.device)
                outputs = self.model(batch_X)
                loss = self.criterion(outputs, batch_y)
                total_loss += loss.item()
                
                _, predicted = torch.max(outputs, 1)
                test_correct += (predicted == batch_y).sum().item()
                test_total += len(batch_y)

                # 记录最后一次测试的结果
                if final == True:
                    for i in range(len(batch_y)):
                        self.test_result[tuple(batch_X[i].tolist())] = (predicted[i] == batch_y[i]).item()

            avg_loss = total_loss / len(self.test_loader)

            test_accuracy = test_correct / test_total * 100
            self.test_accuracy.append(test_accuracy)

        return avg_loss
    
    def final_performance(self):
        """
        打印在测试集上的最终表现
        """
        avg_test_loss = self.evaluate(final=True)
        print(f"Final Test Loss: {avg_test_loss:.4f}, Final Test Accuracy: {self.test_accuracy[config['num_epochs']]:.2f}%")

    def plot_losses_and_accuracy(self):
        """
        绘制训练误差、测试误差、训练准确率和测试准确率曲线
        """
        plt.figure(figsize=(8, 5))
        plt.plot(range(1, self.epochs + 1), self.train_losses, label="Train Loss", color='blue')
        plt.plot(range(1, self.epochs + 1), self.test_losses, label="Test Loss", color='red')
        plt.xlabel("Epoch")
        plt.ylabel("Cross Entropy Loss")
        plt.title("Training and Testing Loss Curve")
        plt.legend()
        plt.grid()

        plt.figure(figsize=(8, 5))
        plt.plot(range(1, self.epochs + 1), self.train_accuracy, label="Train Accuracy", color='blue')
        plt.plot(range(1, self.epochs + 1), self.test_accuracy[0:config["num_epochs"]], label="Test Accuracy", color='red')
        plt.xlabel("Epoch")
        plt.ylabel("Accuracy")
        plt.title("Training and Testing Accuracy Curve")
        plt.legend()
        plt.grid()   

    def plot_result(self):
        """
        绘制训练结果、测试结果散点图
        """
        plt.figure(figsize=(8, 5))
        for position, result in self.train_result.items():
            plt.scatter(position[0], position[1], color=("black" if result else "red"), s=10)
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title("Train Result")
        plt.grid(True)

        plt.figure(figsize=(8, 5))
        for position, result in self.test_result.items():
            plt.scatter(position[0], position[1], color=("black" if result else "red"), s=10)
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title("Test Result")
        plt.grid(True)     

# 主程序
if __name__ == "__main__":
    # 创建数据集
    train_dataset = InsectDataset(config["train_path"])
    test_dataset = InsectDataset(config["test_path"])

    # 绘制数据散点图
    train_dataset.plot_points()
    test_dataset.plot_points()

    # 创建数据加载器
    train_loader = data.DataLoader(train_dataset, batch_size=config["batch_size"], shuffle=True)
    test_loader = data.DataLoader(test_dataset, batch_size=config["batch_size"], shuffle=False)

    # 初始化模型
    model = FNN(input_dim=config["input_dim"])

    # 训练器
    trainer = Trainer(model, train_loader, test_loader, device=config["device"], lr=config["learning_rate"],epochs=config["num_epochs"])

    # 训练模型
    start_time = time.time() # 记录起始时间
    trainer.train()
    end_time = time.time()
    print(f"Running time: {end_time - start_time:.2f}s.")

    # 评估模型
    trainer.final_performance()

    # 绘制误差曲线和准确率曲线
    trainer.plot_losses_and_accuracy()

    # 绘制结果散点图
    trainer.plot_result()

    plt.show() # 统一显示所有图像