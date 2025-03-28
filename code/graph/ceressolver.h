#pragma once

#include <ceres/ceres.h>
#include <defs.h>

namespace CeresSolver {
    // energy functions
    // to use automatic differentiation, we cannot use libraries we don't control in our functor definitions
    // we want to use automatic differentiation because we cannot determine derivatives analytically, and numberical differentiation is slow and inaccurate

    // the energy function corresponding to edge length
    struct EdgeLengthFunctor {
        EdgeLengthFunctor(double edge_length) : edge_length(edge_length) {}
        double edge_length;

        // x1 is a parameter block of size 3 representing position
        // x2 is a parameter block of size 3 representing position
        // edge_length is a parameter block of size 1 representing the ideal length of the edge between x1 and x2
        // residual is the output (size = 1)
        template <typename T>
        bool operator()(const T* const x1, const T* const x2, T* residual) const {
            residual[0] = sqrt((x1[0] - x2[0])*(x1[0] - x2[0]) + (x1[1] - x2[1])*(x1[1] - x2[1]) + (x1[2] - x2[2])*(x1[2] - x2[2])) - edge_length;
            return true;
        }
    };

    // the energy function corresponding to curvature
    struct CurvatureFunctor {
        CurvatureFunctor(int num_neighbours) : num_neighbours(num_neighbours) {}
        int num_neighbours;

        // compute the discrete (uniform) laplacian to estimate curvature
        // parameters is the array of parameter blocks (size = 1 + the number of vertex neighbours)
        // the first parameter block is the vertex position
        // the next n parameter blocks are the positions of the n neighbours
        // residual is the output (size = 1)
        template <typename T>
        bool operator()(T const* const* parameters, T* residual) const {
            T avg_pos[3] = {T(0), T(0), T(0)};
            for (int i = 0; i < num_neighbours; ++i) {
                avg_pos[0] += parameters[i+1][0];
                avg_pos[1] += parameters[i+1][1];
                avg_pos[2] += parameters[i+1][2];
            }

            avg_pos[0] = avg_pos[0] / T(num_neighbours) - parameters[0][0];
            avg_pos[1] = avg_pos[1] / T(num_neighbours) - parameters[0][1];
            avg_pos[2] = avg_pos[2] / T(num_neighbours) - parameters[0][2];

            residual[0] = sqrt(avg_pos[0]*avg_pos[0] + avg_pos[1]*avg_pos[1] + avg_pos[2]*avg_pos[2]);
            return true;
        }
    };

    void run(std::map<size_t, double*>& nodes, std::vector<std::tuple<size_t, size_t, double>>& edges, std::map<size_t, std::vector<size_t>>& neighbours_list, int max_iterations = 50, double lambda = 0.65);
}
