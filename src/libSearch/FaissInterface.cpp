#include "common/easylog++.h"
#include "FaissInterface.h"

// #ifdef CUDA_VERSION
// #include "faiss/gpu/GpuAutoTune.h"
// #include "faiss/gpu/StandardGpuResources.h"
// #include "faiss/gpu/utils/DeviceUtils.h"
// #endif

using namespace std;
using namespace dev;

faissSearch::faissSearch(const string &indexKey, const int dimension, faiss::MetricType metric)
{
    faissIndex.reset(faiss::index_factory(dimension, indexKey.c_str(), metric));
    is_trained = faissIndex->is_trained;
    ntotal = faissIndex->ntotal;
    dim = dimension;
}

bool faissSearch::reset()
{
    try
    {
        faissIndex->reset();
        ntotal = faissIndex->ntotal;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::load(const string &filePath, unordered_map<idx_t, vector<float>> &data)
{
    ifstream in(filePath, ifstream::binary | ifstream::in);
    if (!in.is_open())
    {
        return false;
    }
    int header[5] = {0};
    in.read((char *)header, sizeof(header));
    int dimension = header[0];
    unsigned int count = header[1];
    int fileFormatVersion = header[2];
    if (fileFormatVersion != 1 || dim != dimension)
    {
        in.close();
        return false;
    }
    unsigned int id = 0;
    vector<idx_t> ids;
    ids.resize(count);
    for (size_t i = 0; i < count; i++){
        in.read((char *)&id, sizeof(int));
        ids[i] = static_cast<idx_t>(id);
    }
    for (size_t i = 0; i < count; i++)
    {
        vector<float> feature(dim, 0);
        in.read((char *)feature.data(), dim * sizeof(float));
        data[ids[i]] = move(feature);
    }
    in.close();
    return true;
}

bool faissSearch::load(const string &filePath, vector<idx_t> &ids, vector<float> &data)
{
    ifstream in(filePath, ifstream::binary | ifstream::in);
    if (!in.is_open())
    {
        return false;
    }
    int header[5] = {0};
    in.read((char *)header, sizeof(header));
    int dimension = header[0];
    unsigned int count = header[1];
    int fileFormatVersion = header[2];
    if (fileFormatVersion != 1 || dim != dimension)
    {
        in.close();
        return false;
    }
    data.clear();
    data.resize(count * dim);
    ids.clear();
    ids.resize(count);
    unsigned int id = 0;
    vector<float> feature(dim);
    for (size_t i = 0; i < count;i+=1){
        in.read((char *)&id, sizeof(int));
        ids[i] = static_cast<idx_t>(id);
    }
    for (size_t i = 0; i < count; i++)
    {
        in.read((char *)&data[i * dim], dim * sizeof(float));
    }
    in.close();
    return true;
}

bool faissSearch::write_index(const char *filePath)
{
    try
    {
        faiss::write_index(faissIndex.get(), filePath);
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::read_index(const char *filePath)
{
    try
    {
        faissIndex.reset(faiss::read_index(filePath));
        ntotal = faissIndex->ntotal;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

void faissSearch::train(idx_t n, const float *data)
{
    if (!is_trained)
        faissIndex->train(n, data);
    is_trained = faissIndex->is_trained;
}

void faissSearch::add(const vector<vector<float>> &data)
{
    for (auto item : data)
        faissIndex->add(1, item.data());
    ntotal += data.size();
}

void faissSearch::add(idx_t n, const vector<float> &data)
{

    faissIndex->add(n, data.data());
    ntotal += n;
}

bool faissSearch::add_with_ids(idx_t n, const float *xdata, const long *xids)
{
    try
    {
        faissIndex->add_with_ids(n, xdata, xids);
        ntotal += n;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::search(idx_t n, const float *data, idx_t k, float *distances, long *labels) const
{
    try
    {
        faissIndex->search(n, data, k, distances, labels);
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::search_range(idx_t n, const float *data, float radius, faiss::RangeSearchResult *result)
{
    try
    {
        faissIndex->range_search(n, data, radius, result);
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::remove_ids(const faiss::IDSelector &sel, long &nremove, long &location)
{
    try
    {
        nremove = faissIndex->remove_ids(sel);
        ntotal -= nremove;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::remove_ids_range(const faiss::IDSelector &sel, long &nremove)
{
    try
    {
        nremove = faissIndex->remove_ids(sel);
        ntotal -= nremove;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    return true;
}

bool faissSearch::index_display()
{
    try
    {
        faissIndex->display();
        return true;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
}
