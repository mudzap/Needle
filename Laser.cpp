#include "Laser.h"

Laser::Laser() {
}

Laser::Laser(LaserArgs& laser, TransformArgs& transform)
    : Mesh(laser.nodes * 2), laser(laser), transform(transform) {
    InitLaser();

}

Laser::~Laser() {
}

void Laser::InitLaser() {

    InitTransformBuffers();

    currentNodes = 0;

    initialPosition = transform.position;
    initialVelocity = transform.velocity;
    initialRotation = transform.rotation;
    initialRotationDelta = transform.rotationDelta;

    nodePos.reserve(laser.nodes);
    nodeVel.reserve(laser.nodes);

    nodeNormals.resize(laser.nodes);

    nodeRotation.reserve(laser.nodes);
    nodeRotationDelta.reserve(laser.nodes);
    nodeLifetime.reserve(laser.nodes);

    InitNode();

}

void Laser::SetRotationDelta() {
}

void Laser::InitNode() {

    nodePos.emplace_back(initialPosition);
    nodeVel.emplace_back(initialVelocity);
    nodeRotation.emplace_back(initialRotation);
    nodeRotationDelta.emplace_back(initialRotationDelta);
    nodeLifetime.emplace_back(Timer::countedFrames);

    currentNodes++;
}

void Laser::HandleNodes() {
    if (Timer::countedFrames - nodeLifetime[currentNodes-1] > laser.laserInterval && currentNodes < laser.nodes)
        InitNode();

    Transform::BatchTranslate(nodePos, nodeVel, nodeRotation, currentNodes);

    for (unsigned int i = 0; i < currentNodes; i++)
        nodeNormals[i] = GetNormals(nodeVel[i] * nodeRotation[i]);

    Transform::BatchRotate(nodeRotation, nodeRotationDelta, currentNodes);

    GetVertex();
}

std::array<Complex, 2> Laser::GetNormals(Complex vector) {
    const Complex temp = Complex::Normal(vector) * laser.laserWidth;

    return {
        Complex{-temp.y , temp.x},
        Complex{temp.y , -temp.x}
    };
}


void Laser::GetVertex() {

    vertices.clear();

    size = currentNodes;

    Quad tempQuad = laser.quad;

    const float textConst = 1.f / 1024.f;
    const float uvStep = tempQuad.w / (currentNodes - 1);

    tempQuad.x += 0.5;
    tempQuad.y += 0.5;
    tempQuad.w -= 0.5;
    tempQuad.h -= 0.5;
 
    for (unsigned int i = 0; i < currentNodes; i++) {

        const float xmin = nodePos[i].x + nodeNormals[i][0].x;
        const float ymin = nodePos[i].y + nodeNormals[i][0].y;
        const float xmax = nodePos[i].x + nodeNormals[i][1].x;
        const float ymax = nodePos[i].y + nodeNormals[i][1].y;

        const float umin = textConst * ((tempQuad.x + i * uvStep));
        const float vmin = textConst * (1024.f - (tempQuad.y));
        const float umax = textConst * ((tempQuad.x + i * uvStep));
        const float vmax = textConst * (1024.f - (tempQuad.y + tempQuad.h));

        vertices.emplace_back(std::array<Vertex, 2> {
            Vertex{ { xmin, ymin },{ umin, vmin } },
            Vertex{ { xmax, ymax },{ umax, vmax } },
        });
    }

}

void Laser::ResetDraw() {

    //FIX
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex), &tvertices[0], GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, size * 2);

    glBindVertexArray(0);

}

float Laser::SquaredDistanceToLaserSegment(const Laser& laser, const Complex position, const int ID) {
    const Complex lineSegment = laser.nodePos[ID + 1] - laser.nodePos[ID];
    const Complex vec1Distance = position - laser.nodePos[ID];
    const Complex vec2Distance = position - laser.nodePos[ID + 1];

    if (Complex::Dot(vec1Distance, lineSegment) * Complex::Dot(vec2Distance, lineSegment) < 1) {
        return Complex::SquaredAbs(Complex::Projection(vec1Distance, Complex{ lineSegment.y, -lineSegment.x }));
    }
    else {
        const float vec1DistSquared = Complex::SquaredAbs(vec1Distance);
        const float vec2DistSquared = Complex::SquaredAbs(vec2Distance);
        return (vec1DistSquared < vec2DistSquared) ? std::move(vec1DistSquared) : std::move(vec2DistSquared);
    }

}