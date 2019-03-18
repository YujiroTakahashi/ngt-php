#ifndef __NGT_H__
#define __NGT_H__

#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <NGT/Index.h>

namespace croco {

class Index : public NGT::Index {
public:
    static void create(
        const std::string &path, 
        std::size_t dimension, 
        int edgeSizeForCreation = 10, 
        int edgeSizeForSearch = 40, 
        const std::string distanceType = "L2", 
        const std::string objectType = "Float"
    );
    int getDimension();
    void batchInsert(std::vector<float> data, std::size_t objectCount = 1, std::size_t numThreads = 8);
    int insert(std::vector<float> data);
    std::vector<std::pair<int, float>> search(
        std::vector<float> query,
        std::size_t size = 10,  // the number of resultant objects
        float epsilon = 0.1,    // search parameter epsilon. the adequate range is from 0.0 to 0.15. minus value is acceptable.
        int edgeSize = -1       // the number of used edges for each node during the exploration of the graph.
    );
    std::vector<float> getObject(std::size_t id);
};

} // namespace croco

#endif // #define __NGT_H__
