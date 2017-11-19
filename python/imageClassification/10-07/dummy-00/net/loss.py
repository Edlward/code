from common import *


# loss, accuracy -------------------------
def top_accuracy(probs, labels, top_k=(1,)):
    """Computes the precision@k for the specified values of k"""

    probs  = probs.data
    labels = labels.data

    max_k = max(top_k)
    batch_size = labels.size(0)

    values, indices = probs.topk(max_k, dim=1, largest=True,  sorted=True)
    indices  = indices.t()
    corrects = indices.eq(labels.view(1, -1).expand_as(indices))

    accuracy = []
    for k in top_k:
        # https://stackoverflow.com/questions/509211/explain-slice-notation
        # a[:end]      # items from the beginning through end-1
        c = corrects[:k].view(-1).float().sum(0, keepdim=True)
        accuracy.append(c.mul_(1. / batch_size))
    return accuracy