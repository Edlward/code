#include "cluster.h"

// meanshift 聚类


int main(int argc, char **argv)
{
    Cluster cluster;
    cluster.loadData(100);
    cluster.meanshift();
}