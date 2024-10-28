#ifndef PLANE_H
#define PLANE_H

#include "utility.h"
#include "hittable.h"

class plane : public hittable
{
public:
    plane() {}

    plane(point3 pointMin, point3 pointMax, shared_ptr<material> _mat) :
        pMin(pointMin), pMax(pointMax), rotation(0), mat(_mat)
    {
    };

    plane(point3 pointMin, point3 pointMax, point3 center, shared_ptr<material> _mat) :
        pMin(pointMin), pMax(pointMax), rotation(0), mat(_mat)
    {
    };

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    void transfer(point3 offset)
    {
        pMin = pMin + offset;
        pMax = pMax + offset;
    }

    void rotate(double rot, vec3 rotA)
    {
        rotation = rot;
        rotAxis = unit(rotA);
    }

    private:
        static vec3 rotate_vector_by_quaternion(vec3 v, vec3 qV, float s)
        {
            vec3 newV = (s * s - dot(qV, qV)) * v + 2.0f * qV * dot(qV, v) + 2.0f * s * cross(qV, v);
            return newV;
        }

        bool withoutRotHit(const ray& r, double t_min, double t_max, hit_record& rec) const
        {
            vec3 outward_normal, p;
            double planeMinX, planeMinY, planeMaxX, planeMaxY, planeCoord;
            double intersectionX, intersectionY, t;

            if (pMin.z() == pMax.z()) //xy plane
            {
                planeCoord = pMin.z();
                planeMinX = pMin.x();
                planeMinY = pMin.y();
                planeMaxX = pMax.x();
                planeMaxY = pMax.y();

                outward_normal = vec3(0, 0, 1);

                t = (planeCoord - r.origin().z()) / r.direction().z();
                intersectionX = r.origin().x() + t * r.direction().x();
                intersectionY = r.origin().y() + t * r.direction().y();
            }
            else if (pMin.y() == pMax.y()) //xz plane
            {
                planeCoord = pMin.y();
                planeMinX = pMin.x();
                planeMinY = pMin.z();
                planeMaxX = pMax.x();
                planeMaxY = pMax.z();

                outward_normal = vec3(0, 1, 0);

                t = (planeCoord - r.origin().y()) / r.direction().y();
                intersectionX = r.origin().x() + t * r.direction().x();
                intersectionY = r.origin().z() + t * r.direction().z();
            }
            else if (pMin.x() == pMax.x()) //yz plane
            {
                planeCoord = pMin.x();
                planeMinX = pMin.y();
                planeMinY = pMin.z();
                planeMaxX = pMax.y();
                planeMaxY = pMax.z();

                outward_normal = vec3(1, 0, 0);

                t = (planeCoord - r.origin().x()) / r.direction().x();
                intersectionX = r.origin().y() + t * r.direction().y();
                intersectionY = r.origin().z() + t * r.direction().z();
            }

            if (t < t_min || t > t_max)
                return false;

            if (intersectionX < planeMinX || intersectionX > planeMaxX)
                return false;

            if (intersectionY < planeMinY || intersectionY > planeMaxY)
                return false;

            rec.t = t;
            rec.p = r.at(t);
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat;

            return true;
        }

public:
    shared_ptr<material> mat;
    point3 pMin, pMax;
    vec3 rotAxis;
    double rotation;
};

bool plane::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    if (rotation == 0)
    {
        return withoutRotHit(r, t_min, t_max, rec);
    }
    else
    {
        float rad = degrees_to_radians(-rotation / 2);
        float sinTheta = sin(rad);
        float cosTheta = cos(rad);
        float quaterScal = cosTheta;
        vec3 quaterVec = sinTheta * rotAxis;

        vec3 origin = rotate_vector_by_quaternion(r.origin(), quaterVec, quaterScal);
        vec3 direction = rotate_vector_by_quaternion(r.direction(), quaterVec, quaterScal);
        ray rotatedRay(origin, direction);

        if (!withoutRotHit(rotatedRay, t_min, t_max, rec))
            return false;

        auto p = rotate_vector_by_quaternion(rec.p, -quaterVec, quaterScal);
        rec.p = p;

        auto normal = rotate_vector_by_quaternion(rec.normal, -quaterVec, quaterScal);
        rec.set_face_normal(r, normal);

        return true;
    }

    return false;
}

#endif