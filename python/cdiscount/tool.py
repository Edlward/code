import torch
import shutil
import os

def saveCheckpoint(state, is_best, path):
    '''Save checkpoint, if is_best set copy to best model
    
    Args:
        state: model and other parameter
        is_best: if set, copy to another place
        path: path to save model
    '''
    torch.save(state, os.path.join(path, 'resnetCheckpoing.pth'))
    if is_best:
        shutil.copyfile(os.path.join(path, 'resnetCheckpoing.pth'),
                        os.path.join(path, 'resnetBestModel.pth'))