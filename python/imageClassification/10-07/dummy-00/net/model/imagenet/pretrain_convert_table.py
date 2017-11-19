from common import*

# reference form

#key, pytorch_key
RESNET50_CONVERT_TABLE =[
(	 'layer0.0.conv.weight'	,	 'conv1.weight'	),
(	 'layer0.0.bn.weight'	,	 'bn1.weight'	),
(	 'layer0.0.bn.bias'	,	 'bn1.bias'	),
(	 'layer0.0.bn.running_mean'	,	 'bn1.running_mean'	),
(	 'layer0.0.bn.running_var'	,	 'bn1.running_var'	),
(	 'layer1.0.conv_bn1.conv.weight'	,	 'layer1.0.conv1.weight'	),
(	 'layer1.0.conv_bn1.bn.weight'	,	 'layer1.0.bn1.weight'	),
(	 'layer1.0.conv_bn1.bn.bias'	,	 'layer1.0.bn1.bias'	),
(	 'layer1.0.conv_bn1.bn.running_mean'	,	 'layer1.0.bn1.running_mean'	),
(	 'layer1.0.conv_bn1.bn.running_var'	,	 'layer1.0.bn1.running_var'	),
(	 'layer1.0.conv_bn2.conv.weight'	,	 'layer1.0.conv2.weight'	),
(	 'layer1.0.conv_bn2.bn.weight'	,	 'layer1.0.bn2.weight'	),
(	 'layer1.0.conv_bn2.bn.bias'	,	 'layer1.0.bn2.bias'	),
(	 'layer1.0.conv_bn2.bn.running_mean'	,	 'layer1.0.bn2.running_mean'	),
(	 'layer1.0.conv_bn2.bn.running_var'	,	 'layer1.0.bn2.running_var'	),
(	 'layer1.0.conv_bn3.conv.weight'	,	 'layer1.0.conv3.weight'	),
(	 'layer1.0.conv_bn3.bn.weight'	,	 'layer1.0.bn3.weight'	),
(	 'layer1.0.conv_bn3.bn.bias'	,	 'layer1.0.bn3.bias'	),
(	 'layer1.0.conv_bn3.bn.running_mean'	,	 'layer1.0.bn3.running_mean'	),
(	 'layer1.0.conv_bn3.bn.running_var'	,	 'layer1.0.bn3.running_var'	),
(	 'layer1.0.downsample.conv.weight'	,	 'layer1.0.downsample.0.weight'	),
(	 'layer1.0.downsample.bn.weight'	,	 'layer1.0.downsample.1.weight'	),
(	 'layer1.0.downsample.bn.bias'	,	 'layer1.0.downsample.1.bias'	),
(	 'layer1.0.downsample.bn.running_mean'	,	 'layer1.0.downsample.1.running_mean'	),
(	 'layer1.0.downsample.bn.running_var'	,	 'layer1.0.downsample.1.running_var'	),
(	 'layer1.1.conv_bn1.conv.weight'	,	 'layer1.1.conv1.weight'	),
(	 'layer1.1.conv_bn1.bn.weight'	,	 'layer1.1.bn1.weight'	),
(	 'layer1.1.conv_bn1.bn.bias'	,	 'layer1.1.bn1.bias'	),
(	 'layer1.1.conv_bn1.bn.running_mean'	,	 'layer1.1.bn1.running_mean'	),
(	 'layer1.1.conv_bn1.bn.running_var'	,	 'layer1.1.bn1.running_var'	),
(	 'layer1.1.conv_bn2.conv.weight'	,	 'layer1.1.conv2.weight'	),
(	 'layer1.1.conv_bn2.bn.weight'	,	 'layer1.1.bn2.weight'	),
(	 'layer1.1.conv_bn2.bn.bias'	,	 'layer1.1.bn2.bias'	),
(	 'layer1.1.conv_bn2.bn.running_mean'	,	 'layer1.1.bn2.running_mean'	),
(	 'layer1.1.conv_bn2.bn.running_var'	,	 'layer1.1.bn2.running_var'	),
(	 'layer1.1.conv_bn3.conv.weight'	,	 'layer1.1.conv3.weight'	),
(	 'layer1.1.conv_bn3.bn.weight'	,	 'layer1.1.bn3.weight'	),
(	 'layer1.1.conv_bn3.bn.bias'	,	 'layer1.1.bn3.bias'	),
(	 'layer1.1.conv_bn3.bn.running_mean'	,	 'layer1.1.bn3.running_mean'	),
(	 'layer1.1.conv_bn3.bn.running_var'	,	 'layer1.1.bn3.running_var'	),
(	 'layer1.2.conv_bn1.conv.weight'	,	 'layer1.2.conv1.weight'	),
(	 'layer1.2.conv_bn1.bn.weight'	,	 'layer1.2.bn1.weight'	),
(	 'layer1.2.conv_bn1.bn.bias'	,	 'layer1.2.bn1.bias'	),
(	 'layer1.2.conv_bn1.bn.running_mean'	,	 'layer1.2.bn1.running_mean'	),
(	 'layer1.2.conv_bn1.bn.running_var'	,	 'layer1.2.bn1.running_var'	),
(	 'layer1.2.conv_bn2.conv.weight'	,	 'layer1.2.conv2.weight'	),
(	 'layer1.2.conv_bn2.bn.weight'	,	 'layer1.2.bn2.weight'	),
(	 'layer1.2.conv_bn2.bn.bias'	,	 'layer1.2.bn2.bias'	),
(	 'layer1.2.conv_bn2.bn.running_mean'	,	 'layer1.2.bn2.running_mean'	),
(	 'layer1.2.conv_bn2.bn.running_var'	,	 'layer1.2.bn2.running_var'	),
(	 'layer1.2.conv_bn3.conv.weight'	,	 'layer1.2.conv3.weight'	),
(	 'layer1.2.conv_bn3.bn.weight'	,	 'layer1.2.bn3.weight'	),
(	 'layer1.2.conv_bn3.bn.bias'	,	 'layer1.2.bn3.bias'	),
(	 'layer1.2.conv_bn3.bn.running_mean'	,	 'layer1.2.bn3.running_mean'	),
(	 'layer1.2.conv_bn3.bn.running_var'	,	 'layer1.2.bn3.running_var'	),
(	 'layer2.0.conv_bn1.conv.weight'	,	 'layer2.0.conv1.weight'	),
(	 'layer2.0.conv_bn1.bn.weight'	,	 'layer2.0.bn1.weight'	),
(	 'layer2.0.conv_bn1.bn.bias'	,	 'layer2.0.bn1.bias'	),
(	 'layer2.0.conv_bn1.bn.running_mean'	,	 'layer2.0.bn1.running_mean'	),
(	 'layer2.0.conv_bn1.bn.running_var'	,	 'layer2.0.bn1.running_var'	),
(	 'layer2.0.conv_bn2.conv.weight'	,	 'layer2.0.conv2.weight'	),
(	 'layer2.0.conv_bn2.bn.weight'	,	 'layer2.0.bn2.weight'	),
(	 'layer2.0.conv_bn2.bn.bias'	,	 'layer2.0.bn2.bias'	),
(	 'layer2.0.conv_bn2.bn.running_mean'	,	 'layer2.0.bn2.running_mean'	),
(	 'layer2.0.conv_bn2.bn.running_var'	,	 'layer2.0.bn2.running_var'	),
(	 'layer2.0.conv_bn3.conv.weight'	,	 'layer2.0.conv3.weight'	),
(	 'layer2.0.conv_bn3.bn.weight'	,	 'layer2.0.bn3.weight'	),
(	 'layer2.0.conv_bn3.bn.bias'	,	 'layer2.0.bn3.bias'	),
(	 'layer2.0.conv_bn3.bn.running_mean'	,	 'layer2.0.bn3.running_mean'	),
(	 'layer2.0.conv_bn3.bn.running_var'	,	 'layer2.0.bn3.running_var'	),
(	 'layer2.0.downsample.conv.weight'	,	 'layer2.0.downsample.0.weight'	),
(	 'layer2.0.downsample.bn.weight'	,	 'layer2.0.downsample.1.weight'	),
(	 'layer2.0.downsample.bn.bias'	,	 'layer2.0.downsample.1.bias'	),
(	 'layer2.0.downsample.bn.running_mean'	,	 'layer2.0.downsample.1.running_mean'	),
(	 'layer2.0.downsample.bn.running_var'	,	 'layer2.0.downsample.1.running_var'	),
(	 'layer2.1.conv_bn1.conv.weight'	,	 'layer2.1.conv1.weight'	),
(	 'layer2.1.conv_bn1.bn.weight'	,	 'layer2.1.bn1.weight'	),
(	 'layer2.1.conv_bn1.bn.bias'	,	 'layer2.1.bn1.bias'	),
(	 'layer2.1.conv_bn1.bn.running_mean'	,	 'layer2.1.bn1.running_mean'	),
(	 'layer2.1.conv_bn1.bn.running_var'	,	 'layer2.1.bn1.running_var'	),
(	 'layer2.1.conv_bn2.conv.weight'	,	 'layer2.1.conv2.weight'	),
(	 'layer2.1.conv_bn2.bn.weight'	,	 'layer2.1.bn2.weight'	),
(	 'layer2.1.conv_bn2.bn.bias'	,	 'layer2.1.bn2.bias'	),
(	 'layer2.1.conv_bn2.bn.running_mean'	,	 'layer2.1.bn2.running_mean'	),
(	 'layer2.1.conv_bn2.bn.running_var'	,	 'layer2.1.bn2.running_var'	),
(	 'layer2.1.conv_bn3.conv.weight'	,	 'layer2.1.conv3.weight'	),
(	 'layer2.1.conv_bn3.bn.weight'	,	 'layer2.1.bn3.weight'	),
(	 'layer2.1.conv_bn3.bn.bias'	,	 'layer2.1.bn3.bias'	),
(	 'layer2.1.conv_bn3.bn.running_mean'	,	 'layer2.1.bn3.running_mean'	),
(	 'layer2.1.conv_bn3.bn.running_var'	,	 'layer2.1.bn3.running_var'	),
(	 'layer2.2.conv_bn1.conv.weight'	,	 'layer2.2.conv1.weight'	),
(	 'layer2.2.conv_bn1.bn.weight'	,	 'layer2.2.bn1.weight'	),
(	 'layer2.2.conv_bn1.bn.bias'	,	 'layer2.2.bn1.bias'	),
(	 'layer2.2.conv_bn1.bn.running_mean'	,	 'layer2.2.bn1.running_mean'	),
(	 'layer2.2.conv_bn1.bn.running_var'	,	 'layer2.2.bn1.running_var'	),
(	 'layer2.2.conv_bn2.conv.weight'	,	 'layer2.2.conv2.weight'	),
(	 'layer2.2.conv_bn2.bn.weight'	,	 'layer2.2.bn2.weight'	),
(	 'layer2.2.conv_bn2.bn.bias'	,	 'layer2.2.bn2.bias'	),
(	 'layer2.2.conv_bn2.bn.running_mean'	,	 'layer2.2.bn2.running_mean'	),
(	 'layer2.2.conv_bn2.bn.running_var'	,	 'layer2.2.bn2.running_var'	),
(	 'layer2.2.conv_bn3.conv.weight'	,	 'layer2.2.conv3.weight'	),
(	 'layer2.2.conv_bn3.bn.weight'	,	 'layer2.2.bn3.weight'	),
(	 'layer2.2.conv_bn3.bn.bias'	,	 'layer2.2.bn3.bias'	),
(	 'layer2.2.conv_bn3.bn.running_mean'	,	 'layer2.2.bn3.running_mean'	),
(	 'layer2.2.conv_bn3.bn.running_var'	,	 'layer2.2.bn3.running_var'	),
(	 'layer2.3.conv_bn1.conv.weight'	,	 'layer2.3.conv1.weight'	),
(	 'layer2.3.conv_bn1.bn.weight'	,	 'layer2.3.bn1.weight'	),
(	 'layer2.3.conv_bn1.bn.bias'	,	 'layer2.3.bn1.bias'	),
(	 'layer2.3.conv_bn1.bn.running_mean'	,	 'layer2.3.bn1.running_mean'	),
(	 'layer2.3.conv_bn1.bn.running_var'	,	 'layer2.3.bn1.running_var'	),
(	 'layer2.3.conv_bn2.conv.weight'	,	 'layer2.3.conv2.weight'	),
(	 'layer2.3.conv_bn2.bn.weight'	,	 'layer2.3.bn2.weight'	),
(	 'layer2.3.conv_bn2.bn.bias'	,	 'layer2.3.bn2.bias'	),
(	 'layer2.3.conv_bn2.bn.running_mean'	,	 'layer2.3.bn2.running_mean'	),
(	 'layer2.3.conv_bn2.bn.running_var'	,	 'layer2.3.bn2.running_var'	),
(	 'layer2.3.conv_bn3.conv.weight'	,	 'layer2.3.conv3.weight'	),
(	 'layer2.3.conv_bn3.bn.weight'	,	 'layer2.3.bn3.weight'	),
(	 'layer2.3.conv_bn3.bn.bias'	,	 'layer2.3.bn3.bias'	),
(	 'layer2.3.conv_bn3.bn.running_mean'	,	 'layer2.3.bn3.running_mean'	),
(	 'layer2.3.conv_bn3.bn.running_var'	,	 'layer2.3.bn3.running_var'	),
(	 'layer3.0.conv_bn1.conv.weight'	,	 'layer3.0.conv1.weight'	),
(	 'layer3.0.conv_bn1.bn.weight'	,	 'layer3.0.bn1.weight'	),
(	 'layer3.0.conv_bn1.bn.bias'	,	 'layer3.0.bn1.bias'	),
(	 'layer3.0.conv_bn1.bn.running_mean'	,	 'layer3.0.bn1.running_mean'	),
(	 'layer3.0.conv_bn1.bn.running_var'	,	 'layer3.0.bn1.running_var'	),
(	 'layer3.0.conv_bn2.conv.weight'	,	 'layer3.0.conv2.weight'	),
(	 'layer3.0.conv_bn2.bn.weight'	,	 'layer3.0.bn2.weight'	),
(	 'layer3.0.conv_bn2.bn.bias'	,	 'layer3.0.bn2.bias'	),
(	 'layer3.0.conv_bn2.bn.running_mean'	,	 'layer3.0.bn2.running_mean'	),
(	 'layer3.0.conv_bn2.bn.running_var'	,	 'layer3.0.bn2.running_var'	),
(	 'layer3.0.conv_bn3.conv.weight'	,	 'layer3.0.conv3.weight'	),
(	 'layer3.0.conv_bn3.bn.weight'	,	 'layer3.0.bn3.weight'	),
(	 'layer3.0.conv_bn3.bn.bias'	,	 'layer3.0.bn3.bias'	),
(	 'layer3.0.conv_bn3.bn.running_mean'	,	 'layer3.0.bn3.running_mean'	),
(	 'layer3.0.conv_bn3.bn.running_var'	,	 'layer3.0.bn3.running_var'	),
(	 'layer3.0.downsample.conv.weight'	,	 'layer3.0.downsample.0.weight'	),
(	 'layer3.0.downsample.bn.weight'	,	 'layer3.0.downsample.1.weight'	),
(	 'layer3.0.downsample.bn.bias'	,	 'layer3.0.downsample.1.bias'	),
(	 'layer3.0.downsample.bn.running_mean'	,	 'layer3.0.downsample.1.running_mean'	),
(	 'layer3.0.downsample.bn.running_var'	,	 'layer3.0.downsample.1.running_var'	),
(	 'layer3.1.conv_bn1.conv.weight'	,	 'layer3.1.conv1.weight'	),
(	 'layer3.1.conv_bn1.bn.weight'	,	 'layer3.1.bn1.weight'	),
(	 'layer3.1.conv_bn1.bn.bias'	,	 'layer3.1.bn1.bias'	),
(	 'layer3.1.conv_bn1.bn.running_mean'	,	 'layer3.1.bn1.running_mean'	),
(	 'layer3.1.conv_bn1.bn.running_var'	,	 'layer3.1.bn1.running_var'	),
(	 'layer3.1.conv_bn2.conv.weight'	,	 'layer3.1.conv2.weight'	),
(	 'layer3.1.conv_bn2.bn.weight'	,	 'layer3.1.bn2.weight'	),
(	 'layer3.1.conv_bn2.bn.bias'	,	 'layer3.1.bn2.bias'	),
(	 'layer3.1.conv_bn2.bn.running_mean'	,	 'layer3.1.bn2.running_mean'	),
(	 'layer3.1.conv_bn2.bn.running_var'	,	 'layer3.1.bn2.running_var'	),
(	 'layer3.1.conv_bn3.conv.weight'	,	 'layer3.1.conv3.weight'	),
(	 'layer3.1.conv_bn3.bn.weight'	,	 'layer3.1.bn3.weight'	),
(	 'layer3.1.conv_bn3.bn.bias'	,	 'layer3.1.bn3.bias'	),
(	 'layer3.1.conv_bn3.bn.running_mean'	,	 'layer3.1.bn3.running_mean'	),
(	 'layer3.1.conv_bn3.bn.running_var'	,	 'layer3.1.bn3.running_var'	),
(	 'layer3.2.conv_bn1.conv.weight'	,	 'layer3.2.conv1.weight'	),
(	 'layer3.2.conv_bn1.bn.weight'	,	 'layer3.2.bn1.weight'	),
(	 'layer3.2.conv_bn1.bn.bias'	,	 'layer3.2.bn1.bias'	),
(	 'layer3.2.conv_bn1.bn.running_mean'	,	 'layer3.2.bn1.running_mean'	),
(	 'layer3.2.conv_bn1.bn.running_var'	,	 'layer3.2.bn1.running_var'	),
(	 'layer3.2.conv_bn2.conv.weight'	,	 'layer3.2.conv2.weight'	),
(	 'layer3.2.conv_bn2.bn.weight'	,	 'layer3.2.bn2.weight'	),
(	 'layer3.2.conv_bn2.bn.bias'	,	 'layer3.2.bn2.bias'	),
(	 'layer3.2.conv_bn2.bn.running_mean'	,	 'layer3.2.bn2.running_mean'	),
(	 'layer3.2.conv_bn2.bn.running_var'	,	 'layer3.2.bn2.running_var'	),
(	 'layer3.2.conv_bn3.conv.weight'	,	 'layer3.2.conv3.weight'	),
(	 'layer3.2.conv_bn3.bn.weight'	,	 'layer3.2.bn3.weight'	),
(	 'layer3.2.conv_bn3.bn.bias'	,	 'layer3.2.bn3.bias'	),
(	 'layer3.2.conv_bn3.bn.running_mean'	,	 'layer3.2.bn3.running_mean'	),
(	 'layer3.2.conv_bn3.bn.running_var'	,	 'layer3.2.bn3.running_var'	),
(	 'layer3.3.conv_bn1.conv.weight'	,	 'layer3.3.conv1.weight'	),
(	 'layer3.3.conv_bn1.bn.weight'	,	 'layer3.3.bn1.weight'	),
(	 'layer3.3.conv_bn1.bn.bias'	,	 'layer3.3.bn1.bias'	),
(	 'layer3.3.conv_bn1.bn.running_mean'	,	 'layer3.3.bn1.running_mean'	),
(	 'layer3.3.conv_bn1.bn.running_var'	,	 'layer3.3.bn1.running_var'	),
(	 'layer3.3.conv_bn2.conv.weight'	,	 'layer3.3.conv2.weight'	),
(	 'layer3.3.conv_bn2.bn.weight'	,	 'layer3.3.bn2.weight'	),
(	 'layer3.3.conv_bn2.bn.bias'	,	 'layer3.3.bn2.bias'	),
(	 'layer3.3.conv_bn2.bn.running_mean'	,	 'layer3.3.bn2.running_mean'	),
(	 'layer3.3.conv_bn2.bn.running_var'	,	 'layer3.3.bn2.running_var'	),
(	 'layer3.3.conv_bn3.conv.weight'	,	 'layer3.3.conv3.weight'	),
(	 'layer3.3.conv_bn3.bn.weight'	,	 'layer3.3.bn3.weight'	),
(	 'layer3.3.conv_bn3.bn.bias'	,	 'layer3.3.bn3.bias'	),
(	 'layer3.3.conv_bn3.bn.running_mean'	,	 'layer3.3.bn3.running_mean'	),
(	 'layer3.3.conv_bn3.bn.running_var'	,	 'layer3.3.bn3.running_var'	),
(	 'layer3.4.conv_bn1.conv.weight'	,	 'layer3.4.conv1.weight'	),
(	 'layer3.4.conv_bn1.bn.weight'	,	 'layer3.4.bn1.weight'	),
(	 'layer3.4.conv_bn1.bn.bias'	,	 'layer3.4.bn1.bias'	),
(	 'layer3.4.conv_bn1.bn.running_mean'	,	 'layer3.4.bn1.running_mean'	),
(	 'layer3.4.conv_bn1.bn.running_var'	,	 'layer3.4.bn1.running_var'	),
(	 'layer3.4.conv_bn2.conv.weight'	,	 'layer3.4.conv2.weight'	),
(	 'layer3.4.conv_bn2.bn.weight'	,	 'layer3.4.bn2.weight'	),
(	 'layer3.4.conv_bn2.bn.bias'	,	 'layer3.4.bn2.bias'	),
(	 'layer3.4.conv_bn2.bn.running_mean'	,	 'layer3.4.bn2.running_mean'	),
(	 'layer3.4.conv_bn2.bn.running_var'	,	 'layer3.4.bn2.running_var'	),
(	 'layer3.4.conv_bn3.conv.weight'	,	 'layer3.4.conv3.weight'	),
(	 'layer3.4.conv_bn3.bn.weight'	,	 'layer3.4.bn3.weight'	),
(	 'layer3.4.conv_bn3.bn.bias'	,	 'layer3.4.bn3.bias'	),
(	 'layer3.4.conv_bn3.bn.running_mean'	,	 'layer3.4.bn3.running_mean'	),
(	 'layer3.4.conv_bn3.bn.running_var'	,	 'layer3.4.bn3.running_var'	),
(	 'layer3.5.conv_bn1.conv.weight'	,	 'layer3.5.conv1.weight'	),
(	 'layer3.5.conv_bn1.bn.weight'	,	 'layer3.5.bn1.weight'	),
(	 'layer3.5.conv_bn1.bn.bias'	,	 'layer3.5.bn1.bias'	),
(	 'layer3.5.conv_bn1.bn.running_mean'	,	 'layer3.5.bn1.running_mean'	),
(	 'layer3.5.conv_bn1.bn.running_var'	,	 'layer3.5.bn1.running_var'	),
(	 'layer3.5.conv_bn2.conv.weight'	,	 'layer3.5.conv2.weight'	),
(	 'layer3.5.conv_bn2.bn.weight'	,	 'layer3.5.bn2.weight'	),
(	 'layer3.5.conv_bn2.bn.bias'	,	 'layer3.5.bn2.bias'	),
(	 'layer3.5.conv_bn2.bn.running_mean'	,	 'layer3.5.bn2.running_mean'	),
(	 'layer3.5.conv_bn2.bn.running_var'	,	 'layer3.5.bn2.running_var'	),
(	 'layer3.5.conv_bn3.conv.weight'	,	 'layer3.5.conv3.weight'	),
(	 'layer3.5.conv_bn3.bn.weight'	,	 'layer3.5.bn3.weight'	),
(	 'layer3.5.conv_bn3.bn.bias'	,	 'layer3.5.bn3.bias'	),
(	 'layer3.5.conv_bn3.bn.running_mean'	,	 'layer3.5.bn3.running_mean'	),
(	 'layer3.5.conv_bn3.bn.running_var'	,	 'layer3.5.bn3.running_var'	),
(	 'layer4.0.conv_bn1.conv.weight'	,	 'layer4.0.conv1.weight'	),
(	 'layer4.0.conv_bn1.bn.weight'	,	 'layer4.0.bn1.weight'	),
(	 'layer4.0.conv_bn1.bn.bias'	,	 'layer4.0.bn1.bias'	),
(	 'layer4.0.conv_bn1.bn.running_mean'	,	 'layer4.0.bn1.running_mean'	),
(	 'layer4.0.conv_bn1.bn.running_var'	,	 'layer4.0.bn1.running_var'	),
(	 'layer4.0.conv_bn2.conv.weight'	,	 'layer4.0.conv2.weight'	),
(	 'layer4.0.conv_bn2.bn.weight'	,	 'layer4.0.bn2.weight'	),
(	 'layer4.0.conv_bn2.bn.bias'	,	 'layer4.0.bn2.bias'	),
(	 'layer4.0.conv_bn2.bn.running_mean'	,	 'layer4.0.bn2.running_mean'	),
(	 'layer4.0.conv_bn2.bn.running_var'	,	 'layer4.0.bn2.running_var'	),
(	 'layer4.0.conv_bn3.conv.weight'	,	 'layer4.0.conv3.weight'	),
(	 'layer4.0.conv_bn3.bn.weight'	,	 'layer4.0.bn3.weight'	),
(	 'layer4.0.conv_bn3.bn.bias'	,	 'layer4.0.bn3.bias'	),
(	 'layer4.0.conv_bn3.bn.running_mean'	,	 'layer4.0.bn3.running_mean'	),
(	 'layer4.0.conv_bn3.bn.running_var'	,	 'layer4.0.bn3.running_var'	),
(	 'layer4.0.downsample.conv.weight'	,	 'layer4.0.downsample.0.weight'	),
(	 'layer4.0.downsample.bn.weight'	,	 'layer4.0.downsample.1.weight'	),
(	 'layer4.0.downsample.bn.bias'	,	 'layer4.0.downsample.1.bias'	),
(	 'layer4.0.downsample.bn.running_mean'	,	 'layer4.0.downsample.1.running_mean'	),
(	 'layer4.0.downsample.bn.running_var'	,	 'layer4.0.downsample.1.running_var'	),
(	 'layer4.1.conv_bn1.conv.weight'	,	 'layer4.1.conv1.weight'	),
(	 'layer4.1.conv_bn1.bn.weight'	,	 'layer4.1.bn1.weight'	),
(	 'layer4.1.conv_bn1.bn.bias'	,	 'layer4.1.bn1.bias'	),
(	 'layer4.1.conv_bn1.bn.running_mean'	,	 'layer4.1.bn1.running_mean'	),
(	 'layer4.1.conv_bn1.bn.running_var'	,	 'layer4.1.bn1.running_var'	),
(	 'layer4.1.conv_bn2.conv.weight'	,	 'layer4.1.conv2.weight'	),
(	 'layer4.1.conv_bn2.bn.weight'	,	 'layer4.1.bn2.weight'	),
(	 'layer4.1.conv_bn2.bn.bias'	,	 'layer4.1.bn2.bias'	),
(	 'layer4.1.conv_bn2.bn.running_mean'	,	 'layer4.1.bn2.running_mean'	),
(	 'layer4.1.conv_bn2.bn.running_var'	,	 'layer4.1.bn2.running_var'	),
(	 'layer4.1.conv_bn3.conv.weight'	,	 'layer4.1.conv3.weight'	),
(	 'layer4.1.conv_bn3.bn.weight'	,	 'layer4.1.bn3.weight'	),
(	 'layer4.1.conv_bn3.bn.bias'	,	 'layer4.1.bn3.bias'	),
(	 'layer4.1.conv_bn3.bn.running_mean'	,	 'layer4.1.bn3.running_mean'	),
(	 'layer4.1.conv_bn3.bn.running_var'	,	 'layer4.1.bn3.running_var'	),
(	 'layer4.2.conv_bn1.conv.weight'	,	 'layer4.2.conv1.weight'	),
(	 'layer4.2.conv_bn1.bn.weight'	,	 'layer4.2.bn1.weight'	),
(	 'layer4.2.conv_bn1.bn.bias'	,	 'layer4.2.bn1.bias'	),
(	 'layer4.2.conv_bn1.bn.running_mean'	,	 'layer4.2.bn1.running_mean'	),
(	 'layer4.2.conv_bn1.bn.running_var'	,	 'layer4.2.bn1.running_var'	),
(	 'layer4.2.conv_bn2.conv.weight'	,	 'layer4.2.conv2.weight'	),
(	 'layer4.2.conv_bn2.bn.weight'	,	 'layer4.2.bn2.weight'	),
(	 'layer4.2.conv_bn2.bn.bias'	,	 'layer4.2.bn2.bias'	),
(	 'layer4.2.conv_bn2.bn.running_mean'	,	 'layer4.2.bn2.running_mean'	),
(	 'layer4.2.conv_bn2.bn.running_var'	,	 'layer4.2.bn2.running_var'	),
(	 'layer4.2.conv_bn3.conv.weight'	,	 'layer4.2.conv3.weight'	),
(	 'layer4.2.conv_bn3.bn.weight'	,	 'layer4.2.bn3.weight'	),
(	 'layer4.2.conv_bn3.bn.bias'	,	 'layer4.2.bn3.bias'	),
(	 'layer4.2.conv_bn3.bn.running_mean'	,	 'layer4.2.bn3.running_mean'	),
(	 'layer4.2.conv_bn3.bn.running_var'	,	 'layer4.2.bn3.running_var'	),
(	 'fc.weight'	,	 'fc.weight'	),
(	 'fc.bias' 	,	 'fc.bias' 	),
    ]
