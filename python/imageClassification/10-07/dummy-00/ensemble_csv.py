## https://github.com/colesbury/examples/blob/8a19c609a43dc7a74d1d4c71efcda102eed59365/imagenet/main.py
## https://github.com/pytorch/examples/blob/master/imagenet/main.py

from common import *
from dataset.cdimage import *


## csv tools ############################################################
def ensemble_scores_to_csv(scores,dataset, save_dir):
    num_augments =len(scores)

    df = dataset.df[['_id','num_imgs']]
    df = df.groupby(['_id']).agg({'num_imgs': 'mean'}).reset_index()
    df['cumsum'] = df['num_imgs'].cumsum()

    ids      = df['_id'].values
    num_imgs = df['num_imgs'].values
    cumsum   = df['cumsum'].values
    num_products  = len(ids)
    num_img_files = len(scores[0])

    assert(df['cumsum'].iloc[[-1]].values[0] == num_img_files)
    print('')
    print('making submission csv')
    print('\tnum_products=%d'%num_products)
    print('\tnum_img_files=%d'%num_img_files)
    print('')

    start  = timer()
    labels = []
    probs  = []
    for n in range(num_products):
        if n%10000==0:
            print('\r\t%10d/%d (%0.0f %%)  %0.2f min'%(n, num_products, 100*n/num_products,
                             (timer() - start) / 60), end='',flush=True)
        num = num_imgs[n]
        end = cumsum[n]

        s = []
        for a in range(num_augments):
           for i in range(end-num,end):
                s.append(scores[a][i])

        s = np.array(s).mean(axis=0)
        l = s.argmax()
        labels.append(l)
        probs.append(s[l]/255)
    pass
    print('\n')

    # save results ---
    labels = np.array(labels)
    probs  = np.array(probs)
    df = pd.DataFrame({ '_id' : ids, 'category_id' : labels})
    df['category_id'] = df['category_id'].map(dataset.label_to_category_id)

    return df, labels, probs



def run_ensemble():

    out_dir = '/root/share/project/kaggle/cdiscount/results/excited-resnet50-00c'# s_xx1'
    #out_dir = '/root/share/project/kaggle/cdiscount/results/__old__/resnet50-freeze-03a' # s_xx1'

    ensemble = '160+180'
    augments = ['160','180']
    num_augments = len(augments)

    ## ------------------------------------
    splits=['test_id_v0_420000','test_id_v1_420000','test_id_v2_420000','test_id_v3_508182']
    num_splits = len(splits)

    if 1:
        #split into several parts
        for i in range(num_splits):
            split = splits[i]
            save_dir = out_dir +'/submit/%s/part-%d'%(ensemble,i)
            os.makedirs(save_dir, exist_ok=True)

            test_dataset = CDiscountDataset( split, 'test', mode='test',
                                        transform =[ lambda x:test_augment(x),])

            print('-- np.load --')
            scores=[]
            for a in range(num_augments):
                npy_file  = out_dir + '/submit/%s/part-%d/scores.uint8.npy'%(augments[a],i)
                s = np.load(npy_file)
                scores.append(s)


            print('scores_to_csv')
            df, labels, probs = ensemble_scores_to_csv(scores, test_dataset, save_dir)
            np.save(save_dir + '/labels.npy',labels)
            np.save(save_dir + '/probs.npy',probs)
            np.savetxt(save_dir + '/probs.txt',probs, fmt='%0.5f')
            df.to_csv(save_dir + '/submission_csv.gz', index=False, compression='gzip')

        pass

    ## submission  ----------------------------
    if 1:

        merge_csv_file = out_dir +'/submit/%s/merge_submission_csv.csv.gz'%ensemble
        csv_files = [ out_dir +'/submit/%s/part-%d'%(ensemble,i) + '/submission_csv.gz' for i in range(num_splits)]

        dfs=[]
        for csv_file in csv_files:
            df = pd.read_csv(csv_file, compression='gzip', error_bad_lines=False)
            dfs.append(df)

        merge_df = pd.concat(dfs)
        merge_df.to_csv(merge_csv_file, index=False, compression='gzip')


# main #################################################################
if __name__ == '__main__':
    print( '%s: calling main function ... ' % os.path.basename(__file__))

    run_ensemble()


    print('\nsucess!')