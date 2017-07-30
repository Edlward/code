#include "cluster.h"

int main(int argc, char **argv)
{
    Cluster cluster;
    cluster.loadData(100);
    cluster.meanshift();
}