#include <iostream>
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
 * @param  const char* path
 * @param  int dimension
 * @param  int edgeSizeForCreation
 * @param  int edgeSizeForSearch
 * @param  const char* distanceType
 * @param  const char* objectType
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
 * @param  int rdOnly
 * @return void
 */
void NgtOpen(NgtHandle handle, const char* path, int rdOnly)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    bool readOnly = (rdOnly == 1);
    index->open(path, readOnly);
}

/**
 * Save database
 *
 * @access public
 * @param  NgtHandle handle
 * @return void
 */
void NgtSave(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    index->save();
}

/**
 * Close database
 *
 * @access public
 * @param  NgtHandle handle
 * @return void
 */
void NgtClose(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    index->close();
}

/**
 * Is empty index
 *
 * @access public
 * @return int
 */
int NgtEmptyIndex(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    if (!index->emptyIndex()) {
        return NGT_FALSE;
    }
    return NGT_TRUE;
}

/**
 * get dimension
 *
 * @access public
 * @return int
 */
int NgtGetDimension(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);
    return index->getDimension();
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtExportIndex(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);
    index->exportIndex();
}

/**
 * 
 *
 * @access public
 * @return int
 */
NGTStr NgtGetObjectString(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string output;
    index->getObjectString(&output);

    NGTStr retval = new struct _NGTStr;
    retval->len = output.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, output.c_str());

    return retval;
}

/**
 * 
 *
 * @access public
 * @return int
 */
NGTStr NgtGetDistanceString(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string output;
    index->getDistanceString(&output);

    NGTStr retval = new struct _NGTStr;
    retval->len = output.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, output.c_str());

    return retval;
}

/**
 * 
 *
 * @access public
 * @return int
 */
NGTStr NgtGetLeafNodeString(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string output;
    index->getLeafNodeString(&output);

    NGTStr retval = new struct _NGTStr;
    retval->len = output.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, output.c_str());

    return retval;
}

/**
 * 
 *
 * @access public
 * @return int
 */
NGTStr NgtGetInternalNodeString(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string output;
    index->getInternalNodeString(&output);

    NGTStr retval = new struct _NGTStr;
    retval->len = output.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, output.c_str());

    return retval;
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtSetObjectString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->setObjectString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtSetDistanceString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->setDistanceString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtSetLeafNodeString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->setLeafNodeString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtSetInternalNodeString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->setInternalNodeString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtAddObjectString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->addObjectString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtAddDistanceString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->addDistanceString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtAddLeafNodeString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->addLeafNodeString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtAddInternalNodeString(NgtHandle handle, const char& data)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    std::string strdata(&data);
    index->addInternalNodeString(strdata);
}

/**
 * 
 *
 * @access public
 * @return int
 */
void NgtImportIndex(NgtHandle handle)
{
    croco::Index *index = static_cast<croco::Index*>(handle);

    index->importIndex();
}


/**
 * Insert object
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return void
 */
void NgtInsert(NgtHandle handle, const char* json, size_t len, int objectCount, int numThreads)
{
    croco::Index *index = static_cast<croco::Index*>(handle);
    std::string strjson(json, len);
    nlohmann::json parsed = nlohmann::json::parse(strjson);
    std::vector<float> data;

    for (auto &val : parsed) {
        data.push_back(val);
    }
    index->batchInsert(data, objectCount, numThreads);
}

/**
 * Search object
 *
 * @access public
 * @param  NgtHandle handle
 * @param  const char* path
 * @return int
 */
NGTStr NgtSearch(NgtHandle handle, const char* json, size_t len, int row, float epsilon, int edgeSize)
{
    croco::Index *index = static_cast<croco::Index*>(handle);
    std::string strjson(json, len);
    nlohmann::json parsed = nlohmann::json::parse(strjson);
    std::vector<float> query;

    for (auto &val : parsed) {
        query.push_back(val);
    }

    std::vector<std::pair<int, float>> result = index->search(query, row, epsilon, edgeSize);

    nlohmann::json retj;
    int idx = 0;
    for (const auto &row : result) {
        retj[idx]["Rank"]     = idx + 1;
        retj[idx]["ID"]       = row.first;
        retj[idx]["Distance"] = row.second;
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
