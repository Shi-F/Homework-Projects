import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans

def denoise_with_kmeans(X_train, y_train, k_per_class=3, threshold_multiplier=1.0):
    """
    使用KMeans聚类算法去除数据集中的噪声
    """
    X_clean, y_clean = [], []
    classes = np.unique(y_train)
    
    for c in classes:
        # 提取当前类别的样本
        mask = y_train == c
        X_class = X_train[mask]
        if len(X_class) == 0:
            continue
        
        # 标准化
        scaler = StandardScaler()
        X_scaled = scaler.fit_transform(X_class)
        
        # 使用固定K值3
        kmeans = KMeans(n_clusters=k_per_class, random_state=42)
        clusters = kmeans.fit_predict(X_scaled)
        centers = kmeans.cluster_centers_
        
        # 收集保留样本的索引
        valid_indices = []
        for cluster_id in range(k_per_class):
            cluster_mask = clusters == cluster_id
            X_cluster = X_scaled[cluster_mask]
            if len(X_cluster) < 5:  # 忽略小簇
                continue
            
            # 计算距离并过滤
            distances = np.linalg.norm(X_cluster - centers[cluster_id], axis=1)
            threshold = np.mean(distances) + threshold_multiplier * np.std(distances)
            valid_in_cluster = np.where(distances <= threshold)[0]
            
            # 映射回原始索引
            original_indices = np.where(cluster_mask)[0][valid_in_cluster]
            valid_indices.extend(original_indices.tolist())
        
        # 保留有效样本（原始数据，未标准化）
        X_clean_class = X_class[valid_indices]
        y_clean_class = np.full(len(X_clean_class), c)
        X_clean.append(X_clean_class)
        y_clean.append(y_clean_class)
    
    # 合并数据
    X_clean = np.concatenate(X_clean, axis=0)
    y_clean = np.concatenate(y_clean, axis=0)
    print(f"Deleted {y_train.shape[0] - y_clean.shape[0]} possible issues!")
    return X_clean, y_clean