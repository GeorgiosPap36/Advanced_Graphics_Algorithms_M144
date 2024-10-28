#ifndef CUBOID_H
#define CUBOID_H

#include "utility.h"

#include "hittable_list.h"
#include "plane.h"

class cuboid : public hittable
{
public:
    cuboid() {}

    cuboid(const point3& pMin, const point3& pMax, shared_ptr<material> _mat): cuboidMin(pMin), cuboidMax(pMax), rotation(0), mat(_mat)
    {
        point3 temp = (cuboidMin + cuboidMax) / 2;
        centerOfCuboid = point3(temp.x(), cuboidMin.y(), temp.z());

        createFaces(cuboidMin, cuboidMax, mat);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    void createFaces(const point3& pMin, const point3& pMax, shared_ptr<material> _mat)
    {
        sides.push_back(make_shared<plane>(vec3(cuboidMin.x(), cuboidMin.y(), cuboidMin.z()), vec3(cuboidMax.x(), cuboidMax.y(), cuboidMin.z()), mat)); //same z
        sides.push_back(make_shared<plane>(vec3(cuboidMin.x(), cuboidMin.y(), cuboidMax.z()), vec3(cuboidMax.x(), cuboidMax.y(), cuboidMax.z()), mat));

        sides.push_back(make_shared<plane>(vec3(cuboidMin.x(), cuboidMin.y(), cuboidMin.z()), vec3(cuboidMax.x(), cuboidMin.y(), cuboidMax.z()), mat)); //same y
        sides.push_back(make_shared<plane>(vec3(cuboidMin.x(), cuboidMax.y(), cuboidMin.z()), vec3(cuboidMax.x(), cuboidMax.y(), cuboidMax.z()), mat));

        sides.push_back(make_shared<plane>(vec3(cuboidMin.x(), cuboidMin.y(), cuboidMin.z()), vec3(cuboidMin.x(), cuboidMax.y(), cuboidMax.z()), mat)); //same x
        sides.push_back(make_shared<plane>(vec3(cuboidMax.x(), cuboidMin.y(), cuboidMin.z()), vec3(cuboidMax.x(), cuboidMax.y(), cuboidMax.z()), mat));
    }

    void transfer(point3 offset)
    {
        cuboidMin = cuboidMin + offset;
        cuboidMax = cuboidMax + offset;

        point3 temp = (cuboidMin + cuboidMax) / 2;
        centerOfCuboid = point3(temp.x(), cuboidMin.y(), temp.z());

        for (const auto& side : sides)
        {
            side->transfer(offset);
        }
    }

    void rotate(double rot, vec3 rotA)
    {
        rotation = rot;
        for (const auto& side : sides)
        {
            side->rotate(rot, rotA);
        }
    }

public:
    point3 cuboidMin, cuboidMax, centerOfCuboid;
    std::vector<shared_ptr<plane>> sides;
    shared_ptr<material> mat;
    double rotation;
    vec3 rotAxis;
};

bool cuboid::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{   
    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& side : sides)
    {
        if (side->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}


#endif
