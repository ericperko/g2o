// g2o - General Graph Optimization
// Copyright (C) 2011 R. Kuemmerle, G. Grisetti, H. Strasdat, W. Burgard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef G2O_BASE_EDGE_H
#define G2O_BASE_EDGE_H

#include <iostream>
#include <limits>

#include <Eigen/Core>

#include "optimizable_graph.h"

namespace g2o {

  using namespace Eigen;

  template <int D, typename E>
  class BaseEdge : public OptimizableGraph::Edge
  {
    public:

      static const int Dimension = D;
      typedef E Measurement;
      typedef Matrix<double, D, 1> ErrorVector;
      typedef Matrix<double, D, D> InformationType;

      BaseEdge() : OptimizableGraph::Edge()
      {
        _dimension = D;
        _delta = 1e-9;
      }

      virtual ~BaseEdge() {}

      virtual double chi2() const 
      {
        return _error.dot(information()*_error);
      }

      virtual void robustifyError()
      {
        double nrm = sqrt(_error.dot(information()*_error));
        double w = sqrtOfHuberByNrm(nrm,_huberWidth);
        _error *= w;
      }

      virtual const double* errorData() const { return _error.data();}
      virtual double* errorData() { return _error.data();}
      const ErrorVector& error() const { return _error;}
      ErrorVector& error() { return _error;}

      //! information matrix of the constraint
      const InformationType& information() const { return _information;}
      InformationType& information() { return _information;}
      void setInformation(const InformationType& information) { _information = information;}

      virtual const double* informationData() const { return _information.data();}
      virtual double* informationData() { return _information.data();}

      //! accessor functions for the measurement represented by the edge
      const Measurement& measurement() const { return _measurement;}
      virtual void setMeasurement(const Measurement& m) { _measurement = m;}

      virtual int rank() const {return _dimension;}

      //! accessor functions for the delta used when calculating numeric Jacobians for this edge
      virtual double getDelta() const { return _delta; }
      virtual void setDelta(const double d) { _delta = d; }

      virtual void initialEstimate(const OptimizableGraph::VertexSet&, OptimizableGraph::Vertex*)
      {
        std::cerr << "inititialEstimate() is not implemented, please give implementation in your derived class" << std::endl;
      }

    protected:

      Measurement _measurement;
      InformationType _information;
      ErrorVector _error;

      double _delta; // Delta for calculating numeric Jacobians

    public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };

} // end namespace g2o

#endif
