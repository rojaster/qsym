#ifndef QSYM_DEPENDENCY_H_
#define QSYM_DEPENDENCY_H_

#include <iostream>
#include <memory>
#include <set>
#include <vector>

namespace qsym {

  //  typedef std::set<size_t> DependencySet;
  using DependencySet = std::set<size_t>;
/* @Information(alekum): DependencyNode might not be used
  class DependencyNode {
    public:
      DependencyNode();
      virtual ~DependencyNode();
      DependencySet* getDependencies();
      virtual DependencySet computeDependencies() = 0;

    private:
      DependencySet* dependencies_;
  };
*/
  template<class T>
  class DependencyTree {
    public:
      void addNode(std::shared_ptr<T> node) {
        DependencySet* deps = &node->getDeps(); // node->getDependencies();
        nodes_.push_back(node);
        deps_.insert(deps->begin(), deps->end());
      }

      void merge(const DependencyTree<T>& other) {
        const DependencySet& other_deps = other.getDependencies();
        const std::vector<std::shared_ptr<T>>& other_nodes = other.getNodes();

        nodes_.insert(nodes_.end(), other_nodes.begin(), other_nodes.end());
        deps_.insert(other_deps.begin(), other_deps.end());
      }

      const DependencySet & getDependencies() const {
        return deps_;
      }

      const std::vector<std::shared_ptr<T>>& getNodes() const {
        return nodes_;
      }
    /* 
      friend std::ostream& operator<<(std::ostream& os, const DependencyTree<T>& DT) {
        os << "\tn = [\n";
        for(const auto& node : DT.getNodes()) os << "\t\t" << node->toString() << std::endl;
        os << "\t],\n\td = [ ";
        for(const auto dep : DT.getDependencies()) os << dep << ' ';
        os << "]\n";
      }
    */
    private:
      std::vector<std::shared_ptr<T>> nodes_;
      DependencySet deps_;
  };

  template<class T>
  class DependencyForest {
    public:
      std::shared_ptr<DependencyTree<T>> find(size_t index) {
        if (forest_.size() <= index)
          forest_.resize(index + 1);

        if (forest_[index] == NULL)
          forest_[index] = std::make_shared<DependencyTree<T>>();

        assert(forest_[index] != NULL);
        return forest_[index];
      }

      void addNode(std::shared_ptr<T> node) {
        DependencySet* deps = &node->getDeps(); // node->getDependencies();
        std::shared_ptr<DependencyTree<T>> tree = NULL;
        for (const size_t& index : *deps) {
          std::shared_ptr<DependencyTree<T>> other_tree = find(index);
          if (tree == NULL)
            tree = other_tree;
          else if (tree != other_tree) {
            tree->merge(*other_tree);
            // Update existing reference
            for (const size_t& index : other_tree->getDependencies())
              forest_[index] = tree;
          }
          forest_[index] = tree;
        }
        tree->addNode(node);
      }
    /* 
      friend std::ostream& operator<<(std::ostream& os, const DependencyForest<T>& DF) {
        size_t idx = -1;
        for(const auto& DT : DF.forest_) {
          os << "DT[index=" << ++idx << "] :: {\n";
          if(DT) os << DT;
        }
      }
    */
    private:
      std::vector<std::shared_ptr<DependencyTree<T>>> forest_;
  };

} // namespace qsym

#endif
