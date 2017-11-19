backscatter coefficient 后向散射系数
[极化](https://zhidao.baidu.com/question/433786349728507724.html)

# convNet
<!-- weight_decay=0.01, no data augmentation -->
test Epoch 21 train_loss:0.14493, test_loss 0.19278, best_test_loss 0.19278, accuracy 90.34483    0.3309 
<!-- weight_decay=0.01, resize random crop -->
test Epoch 34, lr: 0.00000 best_test_loss 0.19360, test_accuracy 86.20690, train_loss:0.18985, test_loss 0.19360  0.2705 
<!-- weight_decay=0.01, pad, reiseze random crop -->
test Epoch 43, lr: 0.00000 best_test_loss 0.16528, test_accuracy 93.10345, train_loss:0.25169, test_loss 0.16528 0.1945  **rotate is awesome?**
<!-- weight_decay=0.01, flip, pad, resize random crop -->
test Epoch 37, lr: 0.00000 best_test_loss 0.22386, test_accuracy 91.03448, train_loss:0.27736, test_loss 0.22386  0.2854
<!-- weight_decay=0.01, rotate, flip, pad, random crop-->
test Epoch 29, lr: 0.00000 best_test_loss 0.19650, test_accuracy 88.96552, train_loss:0.31724, test_loss 0.19650  0.2399
<!-- batch_size=64, weight_decay=0.01, speckle noise, pad, resize random crop -->
test Epoch 99, lr: 0.00156250 best_test_loss 0.15662, test_accuracy 86.89655, train_loss:0.17001, test_loss 0.32298   0.1978
<!-- batch_size=64, weight_decay=0.01,  salter and pepper noise, speckle noise, pad, resize random crop -->
test Epoch 99, lr: 0.00004883 best_test_loss 0.15658, test_accuracy 87.58621, train_loss:0.17496, test_loss 0.33669
<!-- batch_size=64, weight_decay=0.001, salter and pepper noise, speckle noise, pad, resize random crop -->
test Epoch 99, lr: 0.00039063 best_test_loss 0.15901, test_accuracy 80.68966, train_loss:0.40502, test_loss 0.48465
<!-- batch_size=128, weight_decay=0.001, salter and pepper noise, speckle noise, pad, resize random crop -->
test Epoch 99, lr: 0.00004883 best_test_loss 0.15368, test_accuracy 85.51724, train_loss:0.20466, test_loss 0.54482
<!-- add min value, batch_size=128, weight_decay=0.001, salter and pepper noise, speckle noise, pad, resize random crop -->  过拟合？？
test Epoch 99, lr: 0.00019531 best_test_loss 0.13829, test_accuracy 91.72414, train_loss:0.14624, test_loss 0.21511  0.2354
<!-- sub min value, batch_size=128, weight_decay=0.001, salter and pepper noise, speckle noise, pad, resize random crop -->  
test Epoch 99, lr: 0.00039063 best_test_loss 0.15874, test_accuracy 86.89655, train_loss:0.10851, test_loss 0.51559
<!-- sub min value, pad, resize random crop -->
test Epoch 99, lr: 0.00002441 best_test_loss 0.13738, test_accuracy 86.89655, train_loss:0.13139, test_loss 0.61632
<!-- speckle filter 没有帮助-->
test Epoch 99, lr: 0.00039063 best_test_loss 0.23778, test_accuracy 83.56164, train_loss:0.18233, test_loss 0.28511
<!-- band_3 sqrt(band_1**2 + band_2**2) 没有帮助-->
test Epoch 99, lr: 0.00625000 best_test_loss 0.22963, test_accuracy 86.30137, train_loss:0.23748, test_loss 0.34087



# resnet18
<!-- pad, resize random crop -->
test Epoch 99, lr: 0.00019531 best_test_loss 0.18321, test_accuracy 91.03448, train_loss:0.08354, test_loss 0.23500 0.2122

<!-- speckle noise, pad, resize random crop -->
