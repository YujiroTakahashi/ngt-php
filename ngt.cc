#include "ngt.h"

namespace croco {

/**
 * Create databse
 *
 * @access public
 * @param   const std::string &path
 * @param   std::size_t dimension
 * @param   int edgeSizeForCreation
 * @param   int edgeSizeForSearch
 * @param   const std::string distanceType
 * @param   const std::string objectType
 * @return void
 */
void Index::create(
    const std::string &path, 
    std::size_t dimension, 
    int edgeSizeForCreation, 
    int edgeSizeForSearch, 
    const std::string distanceType, 
    const std::string objectType
) {
    NGT::Property property;

    property.batchSizeForCreation = 200;
    property.insertionRadiusCoefficient = 0.1 + 1.0;
    property.truncationThreshold = 0;
    property.threadPoolSize = 24;
    property.pathAdjustmentInterval = 0;
    property.dynamicEdgeSizeBase = 30;
    property.buildTimeLimit = 0.0;
    property.objectAlignment = NGT::Property::ObjectAlignmentFalse;
    property.graphType = NGT::Property::GraphType::GraphTypeANNG;
    property.seedType = NGT::Property::SeedType::SeedTypeNone;

    property.dimension = dimension;
    property.edgeSizeForCreation = edgeSizeForCreation;
    property.edgeSizeForSearch = edgeSizeForSearch;

    if (objectType == "Float" || objectType == "float") {
        property.objectType = NGT::Index::Property::ObjectType::Float;
    } else if (objectType == "Byte" || objectType == "byte") {
        property.objectType = NGT::Index::Property::ObjectType::Uint8;
    } else {
        std::cerr << "ngtpy::create: invalid object type. " << objectType << std::endl;
        return;
    }

    if (distanceType == "L1") {
        property.distanceType = NGT::Index::Property::DistanceType::DistanceTypeL1;
    } else if (distanceType == "L2") {
        property.distanceType = NGT::Property::DistanceType::DistanceTypeL2;
    } else if (distanceType == "Hamming") {
        property.distanceType = NGT::Property::DistanceType::DistanceTypeHamming;
    } else if (distanceType == "Angle") {
        property.distanceType = NGT::Property::DistanceType::DistanceTypeAngle;
    } else if (distanceType == "Normalized Angle") {
        property.distanceType = NGT::Property::DistanceType::DistanceTypeNormalizedAngle;
    } else if (distanceType == "Cosine") {
        property.distanceType = NGT::Property::DistanceType::DistanceTypeCosine;
    } else if (distanceType == "Normalized Cosine") {
        property.distanceType = NGT::Property::DistanceType::DistanceTypeNormalizedCosine;
    } else {
        std::cerr << "ngtpy::create: invalid distance type. " << distanceType << std::endl;
        return;
    }

    NGT::Index::createGraphAndTree(path, property);
}

/**
 * get dimension
 *
 * @access public
 * @return int
 */
int Index::getDimension()
{
    NGT::Property prop;
    NGT::Index::getProperty(prop);

    return prop.dimension;
}

/**
 * batch Insert
 *
 * @access public
 * @param   std::vector<float> data
 * @param   std::size_t objectCount
 * @param   std::size_t numThreads
 * @return void
 */
void Index::batchInsert(std::vector<float> data, std::size_t objectCount, std::size_t numThreads)
{
    NGT::Property prop;
    NGT::Index::getProperty(prop);

    if (prop.dimension != data.size()) {
        std::cerr << "ngtpy::insert: Error! dimensions are inconsitency. " << prop.dimension << ":" << data.size() << std::endl;
        return;
    }

    NGT::Index::append(data.data(), objectCount);
    NGT::Index::createIndex(numThreads);
}

/**
 * Insert
 *
 * @access public
 * @param   std::vector<float> data
 * @return int
 */
int Index::insert(std::vector<float> data)
{
    return NGT::Index::insert(data);
}

/**
 * Search
 *
 * @access public
 * @param   std::vector<float> query
 * @param   std::size_t size
 * @param   float epsilon
 * @param   int edgeSize
 * @return std::vector<std::pair<int, double>>
 */
std::vector<std::pair<int, float>> Index::search(std::vector<float> query, std::size_t size, float epsilon, int edgeSize)
{
    std::vector<std::pair<int, float>> result;
    NGT::Object *ngtquery = 0;
    try {
        ngtquery = NGT::Index::allocateObject(static_cast<float*>(query.data()), query.size());
    } catch (NGT::Exception &e) {
        std::cerr << e.what() << endl;
        return result;
    }

    NGT::SearchContainer sc(*ngtquery);
    sc.setSize(size);           // the number of resultant objects.
    sc.setEpsilon(epsilon);     // set exploration coefficient.
    sc.setEdgeSize(edgeSize);   // if maxEdge is minus, the specified value in advance is used.

    NGT::Index::search(sc);

    NGT::Index::deleteObject(ngtquery);
    NGT::ObjectDistances distnces;
    distnces.moveFrom(sc.getWorkingResult());

    for (auto it = distnces.begin(); it != distnces.end(); ++it) {
        result.push_back(std::make_pair(
            (*it).id,
            (*it).distance
        ));
    }

    return result;
}

/**
 * Get object
 *
 * @access public
 * @param   std::size_t id
 * @return std::vector<float>
 */
std::vector<float> Index::getObject(std::size_t id)
{
    NGT::Property prop;
    NGT::Index::getProperty(prop);
    std::vector<float> object;
    object.reserve(prop.dimension);

    if (NGT::ObjectSpace::ObjectType::Uint8 == prop.objectType) {
        auto *obj = static_cast<uint8_t*>(NGT::Index::getObjectSpace().getObject(id));
        for (int i = 0; i < prop.dimension; i++) {
            object.push_back(static_cast<float>(*obj++));
        }
    } else {
        auto *obj = static_cast<float*>(NGT::Index::getObjectSpace().getObject(id));
        for (int i = 0; i < prop.dimension; i++) {
            object.push_back(*obj++);
        }
    }
    return object;
}

} // namespace croco
