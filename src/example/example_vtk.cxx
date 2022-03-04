#include <memory>

#include <viz3d/ui.h>
#include <viz3d/vtk_window.h>

#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkShortArray.h>
#include <vtkStructuredPoints.h>
#include <vtkContourFilter.h>
#include <vtkPointData.h>

#include <Eigen/Dense>

struct TestWindow : viz3d::ImGuiWindow {

    TestWindow(std::string &&winname) : viz3d::ImGuiWindow(std::move(winname)) {}

    void DrawImGUIContent() override {
        ImGui::Text("Hello World");
    }
};

auto GetPointCloudActor() {
    auto poly_data = vtkSmartPointer<vtkPolyData>::New();

    int num_points_per_primitive = 10000;
    int num_primitives = 3;
    int num_points = num_primitives * num_points_per_primitive;

    vtkNew<vtkPoints> points;
    points->SetDataTypeToFloat();
    points->Allocate(num_points);
    points->SetNumberOfPoints(num_points);
    points->GetData()->SetName("Points_XYZ");
    poly_data->SetPoints(points.GetPointer());

    vtkNew<vtkIdTypeArray> cells;
    cells->SetNumberOfValues(num_points * 2);
    vtkIdType *ids = cells->GetPointer(0);
    for (vtkIdType i = 0; i < num_points; ++i) {
        ids[i * 2] = 1;
        ids[i * 2 + 1] = i;
    }

    vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
    cellArray->SetCells(num_points, cells.GetPointer());
    poly_data->SetVerts(cellArray);

    for (auto k(0); k < num_primitives; ++k) {

        auto generate_point = [k]() {
            Eigen::Vector3f random = Eigen::Vector3f::Random();
            random[k] = 0;
            return random;
        };

        for (auto i(0); i < num_points_per_primitive; ++i) {
            Eigen::Vector3f random = generate_point();
            points->SetPoint(k * num_points_per_primitive + i, random.data());
        }
    }

    vtkNew<vtkPolyDataMapper> polydata_mapper;
    polydata_mapper->SetInputData(poly_data.Get());

    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(polydata_mapper);
    return coneActor;
}

auto GetActor() {
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(10);
    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());
    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    return coneActor;
}

int main(int argc, char **argv) {
    auto gui = viz3d::GUI::Instance();
    gui.AddWindow(std::make_shared<TestWindow>("Test Window"));

    auto vtk_window = std::make_shared<viz3d::VTKWindow>("VTK Window");
    vtk_window->InitializeVTKContext();
    vtk_window->AddActor(GetActor());
    gui.AddWindow(vtk_window);

    auto vtk_window2 = std::make_shared<viz3d::VTKWindow>("VTK Window 2");
    vtk_window2->InitializeVTKContext();
    vtk_window2->AddActor(GetPointCloudActor());
    gui.AddWindow(vtk_window2);

    gui.MainLoop();

    return 0;
}