#include "ngt.h"

namespace croco
{

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
    const std::string objectType)
{
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

    NGT::Index::index = new NGT::GraphAndTreeIndex(property);
    NGT::Index::path = path;
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
 * Export Index
 *
 * @access public
 * @return void
 */
void Index::exportIndex()
{
    /* obj */
    _exportObject();

    /* grp */
    _exportDistance();

    /* tre */
    _exportLeafNode();
    _exportInternalNode();
}

/**
 * Get Object string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::getObjectString(std::string* output)
{
    _objectSpace.SerializeToString(output);
}

/**
 * Get Distance string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::getDistanceString(std::string* output)
{
    _distanceSpace.SerializeToString(output);
}

/**
 * Get LeafNode string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::getLeafNodeString(std::string* output)
{
    _leafNodeSpace.SerializeToString(output);
}

/**
 * Get InternalNode string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::getInternalNodeString(std::string* output)
{
    _internalNodeSpace.SerializeToString(output);
}

/**
 * Set Object string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::setObjectString(const std::string& data)
{
    _objectSpace.ParseFromString(data);
}

/**
 * Set Distance string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::setDistanceString(const std::string& data)
{
    _distanceSpace.ParseFromString(data);
}

/**
 * Set LeafNode string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::setLeafNodeString(const std::string& data)
{
    _leafNodeSpace.ParseFromString(data);
}

/**
 * Set InternalNode string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::setInternalNodeString(const std::string& data)
{
    _internalNodeSpace.ParseFromString(data);
}

/**
 * Add Object string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::addObjectString(const std::string& data)
{
    google::protobuf::io::CodedInputStream input_stream(
        reinterpret_cast<const ::google::protobuf::uint8*>(data.data()), data.size()
    );
    _objectSpace.MergeFromCodedStream(&input_stream);
}

/**
 * Add Distance string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::addDistanceString(const std::string& data)
{
    google::protobuf::io::CodedInputStream input_stream(
        reinterpret_cast<const ::google::protobuf::uint8*>(data.data()), data.size()
    );
    _distanceSpace.MergeFromCodedStream(&input_stream);
}

/**
 * Add LeafNode string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::addLeafNodeString(const std::string& data)
{
    google::protobuf::io::CodedInputStream input_stream(
        reinterpret_cast<const ::google::protobuf::uint8*>(data.data()), data.size()
    );
    _leafNodeSpace.MergeFromCodedStream(&input_stream);
}

/**
 * Add InternalNode string
 *
 * @access public
 * @param std::string *output
 * @return void
 */
void Index::addInternalNodeString(const std::string& data)
{
    google::protobuf::io::CodedInputStream input_stream(
        reinterpret_cast<const ::google::protobuf::uint8*>(data.data()), data.size()
    );
    _internalNodeSpace.MergeFromCodedStream(&input_stream);
}

/**
 * Import Index
 *
 * @access public
 * @return void
 */
void Index::importIndex()
{
    /* obj */
    _importObject();

    /* grp */
    _importDistance();

    /* tre */
    _importLeafNode();
    _importInternalNode();
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
        ngtquery = NGT::Index::allocateObject(static_cast<float *>(query.data()), query.size());
    } catch (NGT::Exception &e) {
        std::cerr << e.what() << endl;
        return result;
    }

    NGT::SearchContainer sc(*ngtquery);
    sc.setSize(size);         // the number of resultant objects.
    sc.setEpsilon(epsilon);   // set exploration coefficient.
    sc.setEdgeSize(edgeSize); // if maxEdge is minus, the specified value in advance is used.

    NGT::Index::search(sc);

    NGT::Index::deleteObject(ngtquery);
    NGT::ObjectDistances distnces;
    distnces.moveFrom(sc.getWorkingResult());

    for (auto it = distnces.begin(); it != distnces.end(); ++it) {
        result.push_back(std::make_pair(
            (*it).id,
            (*it).distance));
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
        auto *obj = static_cast<uint8_t *>(NGT::Index::getObjectSpace().getObject(id));
        for (int i = 0; i < prop.dimension; i++) {
            object.push_back(static_cast<float>(*obj++));
        }
    } else {
        auto *obj = static_cast<float *>(NGT::Index::getObjectSpace().getObject(id));
        for (int i = 0; i < prop.dimension; i++) {
            object.push_back(*obj++);
        }
    }
    return object;
}

/**
 * Save Object
 *
 * @access private
 * @return void
 */
void Index::_exportObject()
{
    NGT::ObjectSpace &objectSpace = NGT::Index::getObjectSpace();
    NGT::Repository<NGT::Object> *objects = dynamic_cast<NGT::Repository<NGT::Object> *>(&objectSpace);

    for (NGT::Object *object : *objects) {
        if (0 != object) {
            size_t size = objectSpace.getByteSizeOfObject();
            croco::Object *grain = _objectSpace.add_objects();
            for (size_t idx=0; idx < size; idx++) {
                grain->add_members((uint32_t)(*object)[idx]);
            }
        }
    } // for (NGT::Object *object : *objects)
}

/**
 * Save Distance
 *
 * @access private
 * @return void
 */
void Index::_exportDistance()
{
    NGT::NeighborhoodGraph *nGraph = dynamic_cast<NGT::NeighborhoodGraph *>(NGT::Index::index);

    for (NGT::ObjectDistances *distances : nGraph->repository) {
        if (0 != distances) {
            auto *pbDistances = _distanceSpace.add_distances();
            for (NGT::ObjectDistance distance : *distances) {
                auto *pbDist = pbDistances->add_members();
                pbDist->set_id(distance.id);
                pbDist->set_distance(distance.distance);
            }
        }
    } // for (NGT::ObjectDistances *distances : nGraph->repository)
}

/**
 * Save LeafNode
 *
 * @access private
 * @return void
 */
void Index::_exportLeafNode()
{
    NGT::ObjectSpace &objectSpace = NGT::Index::getObjectSpace();
    NGT::DVPTree *tree = dynamic_cast<NGT::DVPTree *>(NGT::Index::index);

    for (NGT::LeafNode *node : tree->leafNodes) {
        if (0 != node) {
            auto *lNode = _leafNodeSpace.add_nodes();

            lNode->set_id(node->id.getID());
            lNode->set_parent(node->parent.getID());

            for (int i = 0; i < node->objectSize; i++) {
                auto *pbDistances = lNode->add_distances();
                pbDistances->set_id(node->objectIDs[i].id);
                pbDistances->set_distance(node->objectIDs[i].distance);
            }

            if (node->pivot == 0) {
                // Before insertion, parent ID == 0 and object size == 0, that indicates an empty index
                if (node->parent.getID() != 0 || node->objectSize != 0) {
                    NGTThrowException("Node::write: pivot is null!");
                }
            } else {
                NGT::Repository<NGT::Object> *objects = dynamic_cast<NGT::Repository<NGT::Object> *>(&objectSpace);
                size_t size = objectSpace.getByteSizeOfObject();
                for (size_t idx=0; idx < size; idx++) {
                    lNode->add_pivot((uint32_t)(*node->pivot)[idx]);
                }
            }
        } // if (0 != node)
    } // for (NGT::LeafNode *node : tree->leafNodes)
}

/**
 * Save InternalNode
 *
 * @access private
 * @return void
 */
void Index::_exportInternalNode()
{
    NGT::ObjectSpace &objectSpace = NGT::Index::getObjectSpace();
    NGT::DVPTree *tree = dynamic_cast<NGT::DVPTree *>(NGT::Index::index);

    for (NGT::InternalNode *node : tree->internalNodes) {
        if (0 != node) {
            auto *iNode = _internalNodeSpace.add_nodes();

            iNode->set_id(node->id.getID());
            iNode->set_parent(node->parent.getID());


            NGT::Repository<NGT::Object> *objects = dynamic_cast<NGT::Repository<NGT::Object> *>(&objectSpace);
            size_t size = objectSpace.getByteSizeOfObject();
            for (size_t idx=0; idx < size; idx++) {
                iNode->add_pivot((uint32_t)(*node->pivot)[idx]);
            }


            if (node->pivot == 0) {
                NGTThrowException("Node::write: pivot is null!");
            }

            for (size_t i = 0; i < node->childrenSize; i++) {
                iNode->add_children(node->getChildren()[i].getID());
            }

            for (size_t i = 0; i < node->childrenSize - 1; i++){
                iNode->add_borders(node->getBorders()[i]);
            }
        } // if (0 != node)
    } // for (NGT::InternalNode *node : tree->internalNodes)
}

/**
 * Load Object
 *
 * @access private
 * @return void
 */
void Index::_importObject()
{
    NGT::ObjectSpace &objectSpace = NGT::Index::getObjectSpace();
    NGT::Repository<NGT::Object> *objects = dynamic_cast<NGT::Repository<NGT::Object> *>(&objectSpace);
    int size = _objectSpace.objects_size() + 1;

    objects->deleteAll();
    objects->clear();
    objects->reserve(size);
    objects->push_back(0);

    for (int i = 0; i < _objectSpace.objects_size(); i++) {
        const croco::Object &grain = _objectSpace.objects(i);
        NGT::Object *po = new NGT::Object(&objectSpace);
        for (int idx = 0; idx < grain.members_size(); idx++) {
            (*po)[idx] = (uint8_t)grain.members(idx);
        }
        objects->push_back(po);
    } // for (int i = 0; i < _objectSpace.objects_size(); i++)
}

/**
 * Load Distance
 *
 * @access private
 * @return void
 */
void Index::_importDistance()
{
    NGT::NeighborhoodGraph *nGraph = dynamic_cast<NGT::NeighborhoodGraph *>(NGT::Index::index);
    int size = _distanceSpace.distances_size() + 1;

    nGraph->repository.deleteAll();
    nGraph->repository.clear();
    nGraph->repository.reserve(size);
    nGraph->repository.push_back(0);

    for (int i = 0; i < _distanceSpace.distances_size(); i++) {
        NGT::ObjectDistances *objDists = new NGT::ObjectDistances();
        auto &grain = _distanceSpace.distances(i);
        for (int idx = 0; idx < grain.members_size(); idx++) {
            auto &distance = grain.members(idx);
            NGT::ObjectDistance dist(
                distance.id(), 
                distance.distance()
            );
            objDists->push_back(dist);
        }
        nGraph->repository.push_back(objDists);
    } // for (int i = 0; i < _distanceSpace.distances_size(); i++)
}

/**
 * Load LeafNode
 *
 * @access private
 * @return void
 */
void Index::_importLeafNode()
{
    NGT::ObjectSpace &objectSpace = NGT::Index::getObjectSpace();
    NGT::DVPTree *tree = dynamic_cast<NGT::DVPTree *>(NGT::Index::index);
    tree->leafNodes.deleteAll();
    tree->leafNodes.clear();
    tree->leafNodes.push_back(0);

    for (int i = 0; i < _leafNodeSpace.nodes_size(); i++) {
        NGT::LeafNode *node = new NGT::LeafNode();
        const croco::LeafNodeSpace::LeafNode &lNode = _leafNodeSpace.nodes(i);

        node->id.setID(lNode.id());
        node->parent.setID(lNode.parent());

        for (int idx = 0; idx < lNode.distances_size(); idx++) {
            const croco::Distance &distance = lNode.distances(i);
             node->getObjectIDs()[idx].set(
                 distance.id(),
                 distance.distance()
             );
        }

        node->pivot = new NGT::Object(&objectSpace);
        for (int idx = 0; idx < lNode.pivot_size(); idx++) {
            (*node->pivot)[idx] = lNode.pivot(idx);
        } // for (int idx = 0; idx < lNode.pivot_size(); idx++)

        tree->leafNodes.push_back(node);
    } // for (NGT::LeafNode *node : tree->leafNodes)
}

/**
 * Load InternalNode
 *
 * @access private
 * @return void
 */
void Index::_importInternalNode()
{
    NGT::ObjectSpace &objectSpace = NGT::Index::getObjectSpace();
    NGT::DVPTree *tree = dynamic_cast<NGT::DVPTree *>(NGT::Index::index);
    tree->internalNodes.deleteAll();
    tree->internalNodes.clear();
    tree->internalNodes.push_back(0);

    for (int i = 0; i < _internalNodeSpace.nodes_size(); i++) {
        NGT::InternalNode *node = new NGT::InternalNode();
        const croco::InternalNodeSpace::InternalNode &iNode = _internalNodeSpace.nodes(i);

        node->id.setID(iNode.id());
        node->parent.setID(iNode.parent());

        node->pivot = new NGT::Object(&objectSpace);
        for (int idx = 0; idx < iNode.pivot_size(); idx++) {
            (*node->pivot)[idx] = iNode.pivot(idx);
        } // for (int idx = 0; idx < lNode.pivot_size(); idx++)


        node->children = new NGT::Node::ID[iNode.children_size()];
        for (int idx = 0; idx < iNode.children_size(); idx++) {
            node->children[idx].setID(iNode.children(idx));
        }

        node->borders = new NGT::Distance[iNode.borders_size()];
        for (int idx = 0; idx < iNode.borders_size(); idx++) {
            node->borders[idx] = iNode.borders(idx);
        }
    } // for (NGT::InternalNode *node : tree->internalNodes)
}


} // namespace croco
