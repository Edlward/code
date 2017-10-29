<!-- Something Fishy Going On If You're Above This , Ben Hamner  0.99514 -->

# convNet
<!--  -->
epoch 8 train_loss:0.04935, test_loss 0.06783, accuracy 98.55952
<!-- 先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 -->
epoch 9 train_loss:0.02405, test_loss 0.02954, accuracy 99.11905
<!-- 先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 过拟合？？？？-->
epoch 48 train_loss:0.00642, test_loss 0.02636, accuracy 99.29762  
<!-- weight_decay=0.001，先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 过拟合？？？？-->
epoch 49 train_loss:0.01134, test_loss 0.02476, accuracy 99.27381



# resnet18
<!-- learning_rate = 0.001; optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate, weight_decay=0.01) -->
epoch 9 train_loss:0.07411, test_loss 0.10795, accuracy 97.48810
<!-- learning_rate = 0.01; optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate, weight_decay=0.01) -->
epoch 9 train_loss:0.01440, test_loss 0.04922, accuracy 98.50000
<!-- batch_size=256 -->
epoch 9 train_loss:0.02991, test_loss 0.06031, accuracy 98.27381
<!-- 先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 -->
epoch 9 train_loss:0.02105, test_loss 0.03904, accuracy 98.77381
<!-- weight_decay=0.0001, 先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 过拟合？？？？-->
epoch 48 train_loss:0.00474, test_loss 0.02782, accuracy 99.30952  kaggle 0.99457
<!-- weight_decay=0.001，先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 过拟合？？？？-->
epoch 49 train_loss:0.01492, test_loss 0.02077, best_test_loss 0.02077, accuracy 99.42857  params_1.pkl   kaggle 0.99514
<!-- dropout，weight_decay=0.001，先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 差很多-->
epoch 49 train_loss:0.02125, test_loss 0.03317, best_test_loss 0.02283, accuracy 99.04762  
<!-- 减去均值尝试 33.38 weight_decay=0.001，先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28，为什么损失讲下去了，但是准确率没有提升 -->
epoch 49 train_loss:0.01423, test_loss 0.17550, best_test_loss 0.01944, accuracy 94.85714   params_2.pkl   kaggle 0.99457
<!-- 使用全部训练集，减去均值尝试 33.38 weight_decay=0.0005，先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 -->
epoch 2 train_loss:0.00694, test_loss 0.00467, best_test_loss 0.00342, accuracy 99.83333    kaggle 0.99614
<!-- 使用全部训练集 不减均值 weight_decay=0.0005，先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 -->
epoch 37 train_loss:0.00798, test_loss 0.00159, best_test_loss 0.00159, accuracy 99.96429  kaggle 0.99571 应该是过拟合了


# fcNet
<!-- none weight_decay -->
epoch 99 train_loss:0.00061, test_loss 0.10175, accuracy 97.80952
<!-- weight_decay=0.01 -->
epoch 99 train_loss:0.19028, test_loss 0.21005, accuracy 95.11905
<!-- weight_decay=0.001 -->
epoch 99 train_loss:0.02107, test_loss 0.07755, accuracy 97.79762
<!-- weight_decay=0.0001 -->
epoch 99 train_loss:0.00199, test_loss 0.08716, accuracy 97.80952  
<!-- 使用规则化项并不能提升准确率，虽然降低损失 -->

<!-- 先放大32，再random　crop 28-->
epoch 99 train_loss:0.01960, test_loss 0.06247, accuracy 98.04762
<!-- 先放大36，再random crop 28 -->
epoch 99 train_loss:0.03894, test_loss 0.15868, accuracy 95.35714
<!-- 以0.5的概率放大到32或pad到32，再random crop 28 -->
epoch 99 train_loss:0.02671, test_loss 0.04458, accuracy 98.72619
<!-- 先随机旋转(-5,5)度，再以0.5的概率放大到32或pad到32，再random crop 28 -->
epoch 99 train_loss:0.02834, test_loss 0.04011, accuracy 98.78571

<!-- dropout -->








