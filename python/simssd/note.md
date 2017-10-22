```python
(1L, 32L, 36L, 36L)
out = F.relu(self.conv1(x))
out = F.max_pool2d(out, kernel_size=3, padding=0, stride=2)
out = F.relu(self.conv2(out))
outs.append(out)
# lr=0.01, Adam, crop数据集，减去均值 train epoch 29, batch_idx 37/37, batch_loss: 3.152, epoch_average_loss: 3.110
# lr=0.01, Adam, crop数据集，不减均值 train epoch 29, batch_idx 37/37, batch_loss: 3.139, epoch_average_loss: 3.133
# lr=0.01, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 2.711, epoch_average_loss: 2.764

```
```python
(1L, 32L, 33L, 33L)
out = F.relu(self.conv1(x))
out = F.max_pool2d(out, kernel_size=3, padding=0, stride=2)
out = F.relu(self.conv2(out))
out = F.max_pool2d(out, kernel_size=3, padding=0, stride=2)
out = F.relu(self.conv3(out))
# lr=0.01, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 3.231, epoch_average_loss: 3.344
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 3.156, epoch_average_loss: 3.106
```
```python
(1L, 32L, 38L, 38L)
self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1, stride=2)
self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv3 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv4 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
out = F.relu(self.conv1(x))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv2(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv3(out))
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 2.267, epoch_average_loss: 2.261

```

```python
self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1, stride=2)
self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv3 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv4 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv1(x))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv2(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv3(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv4(out))
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 1.609, epoch_average_loss: 1.790

```

```python
self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1, stride=2)
self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv3 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv4 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
self.conv5 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv1(x))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv2(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv3(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv4(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv5(out))
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 1.472, epoch_average_loss: 1.507
```

```python
self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1, stride=2)
self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv3 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv4 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
self.conv5 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv1(x))
out = F.relu(self.conv2(out))
out = F.relu(self.conv3(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv4(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv5(out))
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 1.620, epoch_average_loss: 1.509
```

```python
self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1, stride=2)
self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv3 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2)
self.conv4 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
self.conv5 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv1(x))
out = F.relu(self.conv2(out))
out = F.relu(self.conv3(out))
out = F.relu(self.conv4(out))
out = F.relu(self.conv5(out))
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 1.580, epoch_average_loss: 1.586
```
```python
self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1, stride=2 , dilation=2)
self.conv2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2, dilation=2)
self.conv3 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=2, dilation=2)
self.conv4 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
self.conv5 = nn.Conv2d(32, 32, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv1(x))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv2(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv3(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv4(out))
out = F.max_pool2d(out, kernel_size=3, padding=1, stride=1)
out = F.relu(self.conv5(out))
# lr=0.005, Adam, 原始图片数据集 train epoch 29, batch_idx 37/37, batch_loss: 1.445, epoch_average_loss: 1.599
```
