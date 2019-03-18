#include "ngt_api.h"

/**
 * get croco::NGT::Index version
 *
 * @access public
 * @return int
 */
int NgtVersion()
{
    return NGT_VERSION;
}

/**
 * get croco::NGT::Index size
 *
 * @access public
 * @return int
 */
int NgtSize()
{
    return sizeof(croco::Index);
}

/**
 * create a croco::NGT::Index handle
 *
 * @access public
 * @return NgtHandle
 */
NgtHandle NgtCreate()
{
    NgtHandle handle = new croco::Index;
    return handle;
}

/**
 * free a croco::NGT::Index handle
 *
 * @access public
 * @param  NgtHandle handle
 * @return void
 */
void NgtFree(NgtHandle handle)
{
    delete static_cast<croco::Index*>(handle);
}

/**
 * free a NgtStr handle
 *
 * @access public
 * @param  NgtStr str
 * @return void
 */
void NgtStrFree(NGTStr str)
{
    if (nullptr != str->buff) {
        delete[] str->buff;
    }

    if (nullptr != str) {
        delete str;
    }
}

/**
 * Create database
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return void
 */
void NgtCreateDB(
    const char* path,
    int dimension,
    int edgeSizeForCreation,
    int edgeSizeForSearch,
    const char* distanceType,
    const char* objectType
) {
    std::string spath(path);

    croco::Index::create(
        spath,
        dimension,
        edgeSizeForCreation,
        edgeSizeForSearch,
        std::string(distanceType),
        std::string(objectType)
    );
}

/**
 * Open database
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return void
 */
NGT_API void NgtOpen(NgtHandle handle, const char* path, int rdOnly)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    bool readOnly = (rdOnly == 1);
    index->open(path, readOnly);
}

/**
 * Insert object
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return void
 */
void NgtInsert(NgtHandle handle, float* data, int objectCount, int numThreads)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    int dim = index->getDimension();
    std::vector<float> vdata;

    for (int cnt=0; cnt < objectCount; cnt++) {
        for (int idx=0; idx<dim; idx++) {
            vdata.push_back(data[cnt + idx]);
        }
    }
    index->batchInsert(vdata, objectCount, numThreads);
}

/**
 * Search object
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return int
 */
NGTStr NgtSearch(NgtHandle handle, float* query, int row, float epsilon, int edgeSize)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    int dim = index->getDimension();
    std::vector<float> vquery;
    for (int idx=0; idx<dim; idx++) {
        vquery.push_back(query[idx]);
    }

    std::vector<std::pair<int, float>> result = index->search(vquery, row, epsilon, edgeSize);

    nlohmann::json retj;
    int idx = 0;
    for (const auto &row : result) {
        retj[idx]["Rank"]      = idx + 1;
        retj[idx]["ID"]        = row.first;
        retj[idx]["Distance"]  = row.second;
        idx++;
    }

    std::string retstr = retj.dump();
    NGTStr retval = new struct _NGTStr;
    retval->len = retstr.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, retstr.c_str());

    return retval;
}

/**
 * Remove object
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return int
 */
void NgtRemove(NgtHandle handle, int id)
{
    croco::Index *index = static_cast<croco::Index*>(handle);
    index->remove(id);
}

/**
 * Get object
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return int
 */
NGTStr NgtGetObject(NgtHandle handle, int id)
{
    croco::Index *index = static_cast<croco::Index*>(handle);
    std::vector<float> object = index->getObject(id);

    nlohmann::json retj;
    int idx = 0;
    for (const auto &row : object) {
        retj[idx] = row;
        idx++;
    }

    std::string retstr = retj.dump();
    NGTStr retval = new struct _NGTStr;
    retval->len = retstr.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, retstr.c_str());

    return retval;
}
