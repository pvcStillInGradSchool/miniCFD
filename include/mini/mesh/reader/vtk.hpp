// Copyright 2019 Weicheng Pei and Minghao Yang

#ifndef MINI_MESH_READER_VTK_HPP_
#define MINI_MESH_READER_VTK_HPP_

// For `.vtk` files:
#include <vtkDataSet.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetWriter.h>
// For `.vtu` files:
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
// DataSetAttributes:
#include <vtkFieldData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
// Cells:
#include <vtkCellType.h>  // define types of cells
#include <vtkCellTypes.h>
#include <vtkCell.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkQuad.h>
// Helpers:
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtksys/SystemTools.hxx>

#include <array>
#include <cassert>
#include <string>
#include <memory>
#include <stdexcept>
#include <utility>

namespace mini {
namespace mesh {

template <class Mesh>
  using NodeId = typename Mesh::Node::Id;
class VtkReader {

 public:
  bool ReadFromFile(const std::string& file_name) {
    auto vtk_data_set = Dispatch(file_name.c_str());
    if (vtk_data_set) {
      auto vtk_data_set_owner = vtkSmartPointer<vtkDataSet>();
      vtk_data_set_owner.TakeReference(vtk_data_set);
      mesh_.reset(new Mesh());
      ReadNodes(vtk_data_set);
      ReadCells(vtk_data_set);
      ReadNodeData(vtk_data_set);
      ReadCellData(vtk_data_set);
    } else {
      throw std::runtime_error("ReadFromFile() failed.");
    }
    return true;
  }
  std::unique_ptr<Mesh> GetMesh() {
    auto temp = std::make_unique<Mesh>();
    std::swap(temp, mesh_);
    return temp;
  }

 private:
  void ReadNodes(vtkDataSet* vtk_data_set) {
    int n = vtk_data_set->GetNumberOfPoints();
    for (int i = 0; i < n; i++) {
      auto xyz = vtk_data_set->GetPoint(i);
      mesh_->EmplaceNode(i, xyz[0], xyz[1]);
    }
  }
  void ReadNodeData(vtkDataSet* vtk_data_set) {
  }
  void ReadCells(vtkDataSet* vtk_data_set) {
    int n = vtk_data_set->GetNumberOfCells();
    for (int i = 0; i < n; i++) {
      auto cell = vtk_data_set->GetCell(i);
      auto type = vtk_data_set->GetCellType(i);
      auto id_list = cell_i->GetPointIds();
      switch (type) {
        case /* 1 */VTK_VERTEX: {
          auto a = id_list->GetId(0);
          mesh_->EmplaceCell(i, {a});
          break;
        }
        case /* 3 */VTK_LINE: {
          auto a = id_list->GetId(0);
          auto b = id_list->GetId(1);
          mesh_->EmplaceCell(i, {a, b});
        }
        case /* 5 */VTK_TRIANGLE: {
          auto a = id_list->GetId(0);
          auto b = id_list->GetId(1);
          auto c = id_list->GetId(2);
          mesh_->EmplaceCell(i, {a, b, c});
          break;
        }
        case /* 9 */VTK_QUAD: {
          auto a = id_list->GetId(0);
          auto b = id_list->GetId(1);
          auto c = id_list->GetId(2);
          auto d = id_list->GetId(3);
          mesh_->EmplaceCell(i, {a, b, c, d});
          break;
        }
        case /* 10 */VTK_TETRA: {
        }
        case /* 12 */VTK_HEXAHEDRON: {
        }
        default: {
          assert(false);
        }
      }  // switch (type)
    }  // for each cell
  }
  void ReadCellData(vtkDataSet* vtk_data_set) {
  }
  vtkDataSet* Dispatch(const char* file_name) {
    vtkDataSet* vtk_data_set{nullptr};
    auto extension = vtksys::SystemTools::GetFilenameLastExtension(file_name);
    // Dispatch based on the file extension
    if (extension == ".vtu") {
      vtk_data_set = Read<vtkXMLUnstructuredGridReader>(file_name);
    } else if (extension == ".vtk") {
      vtk_data_set = Read<vtkDataSetReader>(file_name);
    } else {
      throw std::invalid_argument("Unknown extension!");
    }
    return vtk_data_set;
  }
  template <class Reader>
  vtkDataSet* Read(const char* file_name) {
    auto reader = vtkSmartPointer<Reader>::New();
    reader->SetFileName(file_name);
    reader->Update();
    auto vtk_data_set = vtkDataSet::SafeDownCast(reader->GetOutput());
    if (vtk_data_set) {
      vtk_data_set->Register(reader);
    } else {
      throw std::invalid_argument("Fail to read the file!");
    }
    return vtk_data_set;
  }

 private:
  std::unique_ptr<Mesh> mesh_;
};

}  // namespace mesh
}  // namespace mini

#endif  // MINI_MESH_READER_VTK_HPP_