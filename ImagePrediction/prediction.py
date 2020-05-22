# -*- coding: utf-8 -*-
"""
Created on Wed Jul 11 10:18:04 2018

@author: alphatao
"""

from imageai.Prediction import ImagePrediction
import os
import time
#计时
start = time.time()

#当前路径 包含了图片，模型文件
execution_path = os.getcwd()

#创建预测类
prediction = ImagePrediction()

#设置预测模型 有以下四种
#SqueezeNet
prediction.setModelTypeAsSqueezeNet()
prediction.setModelPath(os.path.join(execution_path, "squeezenet_weights_tf_dim_ordering_tf_kernels.h5"))


# ResNet50
# prediction.setModelTypeAsResNet()
# prediction.setModelPath(os.path.join(execution_path, "resnet50_weights_tf_dim_ordering_tf_kernels.h5"))

#InceptionV3
# prediction.setModelTypeAsInceptionV3()
# prediction.setModelPath(os.path.join(execution_path, "inception_v3_weights_tf_dim_ordering_tf_kernels.h5"))

# #DenseNet121
# prediction.setModelTypeAsDenseNet()
# prediction.setModelPath(os.path.join(execution_path, "DenseNet-BC-121-32.h5"))

prediction.loadModel()

#预测图片，以及结果输出数目
predictions, probabilities = prediction.predictImage(os.path.join(execution_path, "1.jpg"), result_count=5 )

#结束计时
end = time.time()

#输出结果
for eachPrediction, eachProbability in zip(predictions, probabilities):
    print(eachPrediction,":",eachProbability)

print ("\ncost time:",end-start)