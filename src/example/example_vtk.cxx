#include <memory>

#include <thread>

#include <viz3d/ui.h>
#include <viz3d/vtk_window.h>

#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkStructuredPoints.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>

#include <Eigen/Dense>


// Define a Custom viz3d::ImGuiWindow to add custom ImGUI Components
struct TestWindow : viz3d::ImGuiWindow {

    TestWindow(std::string &&winname) : viz3d::ImGuiWindow(std::move(winname)) {}

    void DrawImGUIContent() override {
        ImGui::Text("This is a Custom Defined ImGui Window !");
    }
};

// A Point Cloud Actor (fills the content of a PolyData), with a scalar field
auto GetPointCloudActor() {
    auto poly_data = vtkSmartPointer<vtkPolyData>::New();
    int num_points_per_primitive = 10000;
    int num_primitives = 3;
    int num_points = num_primitives * num_points_per_primitive;

    // Setup an array of Points
    vtkNew<vtkPoints> points;
    points->SetDataTypeToFloat();
    points->Allocate(num_points);
    points->SetNumberOfPoints(num_points);
    points->GetData()->SetName("Points_XYZ");
    poly_data->SetPoints(points.GetPointer());

    // Assign for each cell vertex indices
    vtkNew<vtkIdTypeArray> cells;
    cells->SetNumberOfValues(num_points * 2);
    vtkIdType *ids = cells->GetPointer(0);
    for (vtkIdType i = 0; i < num_points; ++i) {
        ids[i * 2] = 1; // num points in the cell = 1
        ids[i * 2 + 1] = i; // pid
    }
    vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
    cellArray->SetCells(num_points, cells.GetPointer());
    poly_data->SetVerts(cellArray);


    // Construct a Scalar field (to see colors)
    vtkNew<vtkFloatArray> scalar_values;
    scalar_values->Allocate(num_points);
    scalar_values->SetNumberOfTuples(num_points);
    scalar_values->SetName("Z");
    poly_data->GetPointData()->AddArray(scalar_values);

    // Generate the points randomly from 3 different planes
    for (auto k(0); k < num_primitives; ++k) {
        auto generate_point = [k]() {
            Eigen::Vector3f random = Eigen::Vector3f::Random();
            random[k] = 0;
            return random;
        };
        for (auto i(0); i < num_points_per_primitive; ++i) {
            Eigen::Vector3f random = generate_point();
            auto idx = k * num_points_per_primitive + i;
            points->SetPoint(idx, random.data());
            scalar_values->SetValue(idx, random.z());
        }
    }
    poly_data->GetPointData()->SetActiveScalars("Z");
    vtkNew<vtkPolyDataMapper> polydata_mapper;
    polydata_mapper->SetInputData(poly_data.Get());

    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(polydata_mapper);

    return coneActor;
}

// Generate a Cone Actor
auto GetConeActor() {
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(20);
    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());
    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    return coneActor;
}

int main(int argc, char **argv) {
    auto &gui = viz3d::GUI::Instance();
    gui.AddWindow(std::make_shared<TestWindow>("Test Window"));

    {
        auto vtk_window = std::make_shared<viz3d::VTKWindow>("VTK Window");
        vtk_window->AddActor(GetConeActor());
        gui.AddWindow(vtk_window);
    }

    size_t window_id;
    {
        auto vtk_window2 = std::make_shared<viz3d::VTKWindow>("VTK Window 2");
        vtk_window2->AddActor(GetPointCloudActor());
        window_id = gui.AddWindow(vtk_window2);
    }

    std::thread gui_thread {viz3d::GUI::LaunchMainLoop, "GUI"};
    gui_thread.join();

    gui.RemoveWindow(window_id);

    return 0;
}