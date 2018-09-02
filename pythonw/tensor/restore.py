import tensorflow as tf
import numpy as np

# 先建立 W, b 的容器
W = tf.Variable(np.arange(6).reshape((2, 3)), dtype=tf.float32, name="weights")
b = tf.Variable(np.arange(3).reshape((1, 3)), dtype=tf.float32, name="biases")

#Notice: init = tf.global_variables_initializer() is unnecessary
saver = tf.train.Saver()
with tf.Session() as sess:
    # 提取变量
    saver.restore(sess,tf.train.latest_checkpoint('folder_for_nn'))
    print("weights:", sess.run(W))
    print("biases:", sess.run(b))
