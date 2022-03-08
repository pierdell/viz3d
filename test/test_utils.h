#ifndef VIZ3D_TEST_UTILS_H
#define VIZ3D_TEST_UTILS_H

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
#include <vtkCellData.h>

#include <Eigen/Dense>

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

// Get Lines Actor
auto GetLineActor() {
    const auto num_lines = 10;
    const auto num_points = num_lines + 1;

    auto poly_data = vtkSmartPointer<vtkPolyData>::New();

    // Setup the array of points
    auto vtk_points = vtkSmartPointer<vtkPoints>::New();
    vtk_points->SetDataTypeToFloat();
    vtk_points->Allocate(num_points);
    vtk_points->SetNumberOfPoints(num_points);
    vtk_points->GetData()->SetName("Points_XYZ");
    poly_data->SetPoints(vtk_points.GetPointer());

    // Assign for each cell vertex indices
    auto cell_ids = vtkSmartPointer<vtkIdTypeArray>::New();
    cell_ids->SetNumberOfValues(num_lines * 3);
    vtkIdType *ids = cell_ids->GetPointer(0);
    for (vtkIdType line_id = 0; line_id < num_lines; ++line_id) {
        auto cell_id = line_id * 3;
        ids[cell_id] = 2; // num points in the cell = 2
        ids[cell_id + 1] = line_id; // origin pid
        ids[cell_id + 2] = line_id + 1; // pid of the endpoint of the line
    }

    double scale = 5.;

    vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
    cellArray->SetCells(num_lines, cell_ids.GetPointer());
    poly_data->SetLines(cellArray);

    auto color_field = vtkSmartPointer<vtkFloatArray>::New();
    color_field->Allocate(num_lines);
    color_field->SetName("Scalar");
    color_field->SetNumberOfTuples(num_points);
    poly_data->GetCellData()->AddArray(color_field);

    Eigen::Vector3f previous_point = Eigen::Vector3f::Random() * scale;
    for (auto line_idx(0); line_idx < num_lines; line_idx++) {

        if (line_idx == 0)
            vtk_points->SetPoint(0, previous_point.data());

        Eigen::Vector3f new_point = Eigen::Vector3f::Random() * scale;
        vtk_points->SetPoint(line_idx + 1, new_point.data());
        previous_point = new_point;

        color_field->SetValue(line_idx, float(line_idx) / (num_lines - 1));
    }
    poly_data->GetCellData()->SetActiveScalars("Scalar");


    vtkNew<vtkPolyDataMapper> polydata_mapper;
    polydata_mapper->SetInputData(poly_data.Get());

    vtkNew<vtkActor> poly_data_actor;
    poly_data_actor->SetMapper(polydata_mapper);

    return poly_data_actor;
}

#endif //VIZ3D_TEST_UTILS_H
