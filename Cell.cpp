#include "Cell.h"



Cell::Cell()
{
}

Cell::Cell(const std::vector<Face*>& faces)
{
	faceList = faces;
	for (auto face : faceList) {
		face->setAdjacient(this);
	}
	computeCenter();
	computeVolume();
}


Cell::~Cell()
{
}

const double Cell::computeVolume() {
	volume = 0;
	for (auto face : getExtendedFaceList()) {
		const double fA = face->getArea();
		const Eigen::Vector3d fn = face->getNormal();
		double h = std::abs(fn.dot(center - face->getCenter()));
		double dV = 1. / 3. * fA * h;
		assert(dV > 0);
		volume += dV;
	}
	assert(volume > 0);
	return volume;
}

const double Cell::getVolume() const
{
	return volume;
}

const std::vector<Face*> Cell::getFaceList() const
{
	return faceList;
}

const std::vector<Face*> Cell::getExtendedFaceList() const {
	std::vector<Face*> extendedFaceList;
	for (Face* face : faceList) {
		if (face->getSubFaceList().size() > 0) {
			for (Face* subFace : face->getSubFaceList()) {
				extendedFaceList.push_back(subFace);
			}
		}
		else {
			extendedFaceList.push_back(face);
		}
	}
	return extendedFaceList;
}

const std::vector<Edge*> Cell::getEdgeList() const {
	if (edgeList.size() == 0) {
		for (const Face* face : faceList) {
			for (Edge* edge : face->getEdgeList()) {
				edgeList.push_back(edge);
			}
		}
		std::sort(edgeList.begin(), edgeList.end());
		edgeList.erase(std::unique(edgeList.begin(), edgeList.end()), edgeList.end());
	}
	assert(edgeList.size() <= 18);
	return edgeList;
}

bool Cell::hasFace(Face * face) const
{
	assert(face != nullptr);
	for (auto f : faceList) {
		if (f == face) {
			return true;
		}
	}
	return false;
}

const int Cell::getOrientation(const Face * face) const
{
	assert(face != nullptr);
	assert(isMemberFace(face)); 
	int orientation;
	if (face->isPlane()) {
		const Eigen::Vector3d a = face->getCenter() - this->center;
		const Eigen::Vector3d fn = face->getNormal();
		orientation = (a.dot(fn) > 0) ? 1 : -1;
	}
	else {
		const Eigen::Vector3d a = face->getSubFaceList()[0]->getCenter() - this->center;
		const Eigen::Vector3d fn = face->getSubFaceList()[0]->getNormal();
		orientation = (a.dot(fn) > 0) ? 1 : -1;
	}
	return orientation;
}

const Eigen::Vector3d Cell::computeCenter() {
	// determine cell center
	std::vector<Face*> zFaces;
	for (auto face : getExtendedFaceList()) {
		const Eigen::Vector3d fn = face->getNormal();
		if (fn.cross(Eigen::Vector3d(0, 0, 1)).norm() < 100 * std::numeric_limits<double>::epsilon()) {
			zFaces.push_back(face);
		}
	}
	assert(zFaces.size() == 2); // In our setup, each (dual or primal) cell has exactly two (sub-)faces that are perpendicular to z-axis.
	assert(abs(zFaces[0]->getArea() - zFaces[1]->getArea()) < 1e-10);
	center = 1. / 2. * (zFaces[0]->getCenter() + zFaces[1]->getCenter());
	// this block is for degugging.
	/*
	if (n_zFaces < 2) {
		for (auto face : faceList) {
			const Eigen::Vector3d fn = face->getNormal();

			const double fn_x_zUnit_norm = fn.cross(Eigen::Vector3d(0, 0, 1)).norm();
			std::cout << "Face " << face->getIndex() << ": " << "(fn x zUnit).norm() = " << fn_x_zUnit_norm << std::endl;
			if (fn.cross(Eigen::Vector3d(0, 0, 1)).norm() < 100 * std::numeric_limits<double>::epsilon()) {
				zFaces.push_back(face);
			}
		}
	}
	assert(zFaces.size() == 2);*/
	//std::cout << "z-Faces: ";
	//for (auto f : zFaces) {
	//	std::cout << f->getIndex() << ",";
	//}
	//std::cout << std::endl;
	/*
	assert(n_zFaces <= 2);
	if (n_zFaces == 2) {
		center = Eigen::Vector3d::Zero();
		center = 1. / 2. * (zFaces[0]->getCenter() + zFaces[1]->getCenter());
	}
	else {
		assert(faceList.size() == 5);
		center = Eigen::Vector3d::Zero();
		for (Face* f : faceList) {
			center += f->getCenter();
		}
		center /= faceList.size();
	}*/

	return center;
}

const Eigen::Vector3d Cell::getCenter() const
{
	return center;
}

const Eigen::MatrixXd Cell::getVertexCoordinates() const
{
	std::vector<Vertex*> vertexList;
	for (auto face : faceList) {
		for (auto vertex : face->getVertexList()) {
			vertexList.push_back(vertex);
		}
	}
	std::sort(vertexList.begin(), vertexList.end());    // ytw: How does it work? vertexList contains Vertex*!
	std::vector<Vertex*>::iterator it;
	it = std::unique(vertexList.begin(), vertexList.end());
	int nVertices = std::distance(vertexList.begin(), it);
	assert(nVertices >= 4);

	Eigen::MatrixXd coords(nVertices, 3);
	for (int i = 0; i < nVertices; i++) {
		const Vertex * vertex = vertexList[i];
		coords.row(i) = vertex->getPosition();
	}
	return coords;
}

void Cell::setFluidType(const FluidType & type)
{
	this->fluidType = type;
}

const Cell::FluidType Cell::getFluidType() const
{
	return this->fluidType;
}

const bool Cell::isMemberFace(const Face* face) const {
	for (const Face* f : faceList) {
		if (f == face) return true;
	}
	for (const Face* f : getExtendedFaceList()) {
		if (f == face) return true;
	}
	return false;
}
