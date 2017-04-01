#module docstring
import mymnist_loader as ml
import mynetwork

print('hello python')


# training_data, validation_data, test_data = ml.load_data()
# print(training_data)
# print('len(training_data): ',len(training_data))
# print('len(training_data[0]): ', len(training_data[0]))
# print('len(training_data[1]): ', len(training_data[1]))


train, validation, test = ml.load_data_wrapper()

# print(train[0])
print('type(train): ', type(train))
print('len(training): ', len(train))
print('len(train[0]): ', len(train[0]))
print('len(train[0][0]): ', len(train[0][0]))
print('len(train[0][1]): ', len(train[0][1]))
print()


print('len(validation[0]): ', len(validation[0]))
print('len(validation[0][0]): ', len(validation[0][0]))
print('validation[0][1]): ', validation[0][1])
# x = zip(1, 2)
# print(x)

print()
#784*100*10
net = mynetwork.Network([784, 30, 10])
#30:epochs, 10:mini_batch, eta:3.0
net.SGD(train, 30, 10, 3.0, test)

