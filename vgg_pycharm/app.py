#coding:utf-8
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import vgg16
import utils
from Nclasses import labels

#img_path = input('Input the path and image name:')
img_path = '0.jpg'
img_ready = utils.load_image(img_path) 

fig=plt.figure(u"Top-5 预测结果") 

#预测

# with tf.Session() as sess:
#     images = tf.placeholder(tf.float32, [1, 224, 224, 3])
#     vgg = vgg16.Vgg16()
#     vgg.forward(images)
#     probability = sess.run(vgg.prob, feed_dict={images:img_ready})
#     top5 = np.argsort(probability[0])[-1:-6:-1]
#     print ("top5:",top5)
#     values = []
#     bar_label = []
#     for n, i in enumerate(top5):
#         values.append(probability[0][i])
#         bar_label.append(labels[i])
#         print (i, ":", labels[i], "----", utils.percent(probability[0][i]) )
#     ax = fig.add_subplot(111)
#     ax.bar(range(len(values)), values, tick_label=bar_label, width=0.5, fc='g')
#     ax.set_ylabel(u'probabilityit')
#     ax.set_title(u'Top-5')
#     for a,b in zip(range(len(values)), values):
#         ax.text(a, b+0.0005, utils.percent(b), ha='center', va = 'bottom', fontsize=7)
#     plt.show()

#可视化每一层

from scipy.linalg import norm
#特征提取可视化

img1 = utils.load_image("0.jpg")
batch1 = img1.reshape((1, 224, 224, 3))

def get_row_col(num_pic):
    '''
    计算行列的值
    :param num_pic: 特征图的数量
    :return:
    '''
    squr = num_pic ** 0.5
    row = round(squr)
    col = row + 1 if squr - row > 0 else row
    return row, col

def visualize_feature_map(feature_batch):
    '''
    创建特征子图，创建叠加后的特征图
    :param feature_batch: 一个卷积层所有特征图
    :return:
    '''
    feature_map = np.squeeze(feature_batch, axis=0)

    feature_map_combination = []
    plt.figure(figsize=(8, 7))

    # 取出 featurn map 的数量，因为特征图数量很多，这里直接手动指定了。
    #num_pic = feature_map.shape[2]

    row, col = get_row_col(25)
    # 将 每一层卷积的特征图，拼接层 5 × 5
    for i in range(0, 25):
        feature_map_split = feature_map[:, :, i]
        feature_map_combination.append(feature_map_split)
        plt.subplot(row, col, i+1)
        plt.imshow(feature_map_split)
        plt.axis('off')

    plt.show()

with tf.device('/cpu:0'):
    with tf.Session() as sess:
        images = tf.placeholder("float", [1, 224, 224, 3])
        feed_dict = {images: batch1}

        vgg = vgg16.Vgg16()
        with tf.name_scope("content_vgg"):
            vgg.forward(images)
        feature = sess.run(vgg.conv3_1, feed_dict=feed_dict)# 需要提取哪一层特征，就在这里做修改，比如fc6，只需要把vgg.fc7修改为vgg.fc6
    visualize_feature_map(feature)
