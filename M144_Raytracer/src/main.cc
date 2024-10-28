#include <iostream>
#include <chrono>
#include <execution>

#include "sdlhelper.h"
#include "image.h"

#include "utility.h"

#include "camera.h"
#include "color.h"
#include "material.h"

#include "hittable_list.h"

#include "sphere.h"
#include "cuboid.h"


color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;
    // If we have exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return color(0, 0, 0);
    }
    if (world.hit(r, 0.001, inf, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

// Scenes
hittable_list scene1()
{
    hittable_list sceneObjects;
    sceneObjects.add(make_shared<plane>(point3(-100, 0, -100), point3(100, 0, 100), make_shared<diffuse>(color(0.2, 0.2, 0.2))));

    auto glass_material = make_shared<dielectric>(1.5);
    auto metal_material = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto diffuse_material = make_shared<diffuse>(color(0.7, 0.6, 0.5));

    ///Cuboid 1
    point3 offset = point3(4.75, 0, -2.25);
    point3 min = point3(0, 0, 0) + offset;
    point3 max = point3(1.5, 1.5, 1.5) + offset;
    shared_ptr<cuboid> cuboid1 = make_shared<cuboid>(min, max, glass_material);
    sceneObjects.add(cuboid1);

    ///Cuboid 2
    offset = point3(-2, 0, -1);
    min = point3(0, 0, 0) + offset;
    max = point3(1, 2.5, 5) + offset;
    shared_ptr<cuboid> cuboid2 = make_shared<cuboid>(min, max, metal_material);
    sceneObjects.add(cuboid2);

    ///Cuboid 3
    offset = point3(1, 0, 2.5);
    min = point3(0, 0, 0) + offset;
    max = point3(2, 1.1, 2) + offset;
    shared_ptr<cuboid> cuboid3 = make_shared<cuboid>(min, max, diffuse_material);
    sceneObjects.add(cuboid3);

    for (int i = -3; i < 3; i++)
    {
        auto x = 3 + (random_double() - 0.5) * 2;
        auto z = -i * clamp(0.7, 1, random_double());
        offset = point3(x, 0, z);
        min = point3(0, 0, 0) + offset;
        max = point3(clamp(0.2, 0.5, random_double()), clamp(0.2, 0.5, random_double()), clamp(0.2, 0.5, random_double())) + offset;

        auto choose_mat = random_double();
        shared_ptr<material> mat;

        if (choose_mat < 0.8)
        {
            auto randomColor = color::random() * color::random();
            mat = make_shared<diffuse>(randomColor);
            sceneObjects.add(make_shared<cuboid>(min, max, mat));
        }
        else
        {
            sceneObjects.add(make_shared<cuboid>(min, max, metal_material));
        }
    }

    return sceneObjects;
}

hittable_list scene2()
{
    hittable_list sceneObjects;
    sceneObjects.add(make_shared<plane>(point3(-100, 0, -100), point3(100, 0, 100), make_shared<diffuse>(color(0.2, 0.2, 0.2))));

    auto glass_material = make_shared<dielectric>(1.5);
    auto metal_material = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto diffuse_material = make_shared<diffuse>(color(0.7, 0.3, 0.3));

    ///Cuboid 1
    point3 offset = point3(1, 1.5, 0);
    point3 min = point3(-1.5, -1.5, -1.5) + offset;
    point3 max = point3(1.5, 1.5, 1.5) + offset;
    shared_ptr<cuboid> cuboid1 = make_shared<cuboid>(min, max, glass_material);
    cuboid1->rotate(25, vec3(0, 1, 0));
    sceneObjects.add(cuboid1);

    ///Cuboid 2
    offset = point3(1.5, 1.5, 2);
    min = point3(-0.5, -0.5, -0.25) + offset;
    max = point3(1.5, 0.5, 0.5) + offset;
    shared_ptr<cuboid> cuboid2 = make_shared<cuboid>(min, max, metal_material);
    cuboid2->rotate(30, vec3(1, 1, 1));
    sceneObjects.add(cuboid2);

    ///Cuboid 3
    offset = point3(2, 0.5, 2);
    min = point3(-0.5, -0.5, -0.25) + offset;
    max = point3(1.5, 1.5, 0.5) + offset;
    shared_ptr<cuboid> cuboid3 = make_shared<cuboid>(min, max, make_shared<diffuse>(color(0.5, 0.8, 0.3)));
    cuboid3->rotate(75, vec3(1, 0, 1));
    sceneObjects.add(cuboid3);

    ///Sphere 1
    point3 center = point3(0, 1.5, 0);
    shared_ptr<sphere> sphere1 = make_shared<sphere>(center, 1, diffuse_material);
    sceneObjects.add(sphere1);

    for (int i = -3; i < 3; i++)
    {
        auto x = 3 + (random_double() - 0.5) * 2;
        auto z = -i * clamp(0.7, 1, random_double());
        offset = point3(x, 0, z);
        min = point3(0, 0, 0) + offset;
        max = point3(clamp(0.2, 0.5, random_double()), clamp(0.2, 0.5, random_double()), clamp(0.2, 0.5, random_double())) + offset;

        auto randomColor = color::random();
        shared_ptr<material> mat = make_shared<diffuse>(randomColor);
        shared_ptr<cuboid> randomCuboid = make_shared<cuboid>(min, max, mat);
        //randomCuboid->transfer(-point3(offset.x(), min.y(), offset.z()));
        randomCuboid->rotate(random_double() * 100, vec3(0, 1, 0));
        sceneObjects.add(randomCuboid);
    }

    return sceneObjects;
}

hittable_list scene3()
{
    hittable_list sceneObjects;
    sceneObjects.add(make_shared<plane>(point3(-100, 0, -100), point3(100, 0, 100), make_shared<diffuse>(color(0.2, 0.2, 0.2))));

    auto glass_material = make_shared<dielectric>(1.5);
    auto metal_material = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto diffuse_material = make_shared<diffuse>(color(0.7, 0.3, 0.3));

    ///Cuboid 1
    point3 offset = point3(0, 1.5, 0);
    point3 min = point3(-1.5, -1.5, -1.5) + offset;
    point3 max = point3(1.5, 1.5, 1.5) + offset;
    shared_ptr<cuboid> cuboid1 = make_shared<cuboid>(min, max, glass_material);
    cuboid1->rotate(60, vec3(0, 1, 0));
    sceneObjects.add(cuboid1);

    ///Sphere 1
    point3 center = point3(0, 1.5, 0);
    shared_ptr<sphere> sphere1 = make_shared<sphere>(center, 1, diffuse_material);
    sceneObjects.add(sphere1);
    return sceneObjects;
}

int main() {

    int numberOfThreads = 8; //Thread number//

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 768;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 20;
    const int max_depth = 10;

    // World
    hittable_list world = scene3();

    // Camera
    point3 lookfrom = point3(12, 3, 0);
    point3 lookat = point3(0, 0, 0);
    vec3 vup = point3(0, 1, 0);
    auto vfov = 30.0;
    auto aperture = 0.1;
    auto dist_to_focus = 10.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

    // Array to save the color of each pixel
    int*** pixelArray;
    pixelArray = new int** [image_height];
    for (int x = 0; x < image_height; ++x)
    {
        pixelArray[x] = new int* [image_width];
        for (int y = 0; y < image_width; ++y)
        {
            pixelArray[x][y] = new int[3];
            for (int z = 0; z < 3; ++z)
            {
                pixelArray[x][y][z] = 0;
            }
        }
    }

    //Splitting the calculations to threads
    std::vector<int> imageHorizontalIter, threadsIter;

    imageHorizontalIter.resize(image_width);
    threadsIter.resize(numberOfThreads);

    for (int i = 0; i < image_width; i++)
        imageHorizontalIter[i] = i;

    for (int i = 0; i < numberOfThreads; i++)
        threadsIter[i] = i;

    std::vector<std::vector<int>> imageVerticalIter;
    imageVerticalIter.resize(numberOfThreads);

    int temp = static_cast<int>(image_height / numberOfThreads);

    for (int i = 0; i < numberOfThreads; i++)
    {
        if (i < numberOfThreads - 1)
        {
            imageVerticalIter[i].resize(temp);
            for (int j = 0; j < temp; j++)
            {
                imageVerticalIter[i][j] = i * temp + j;
            }
        }
        else
        {
            imageVerticalIter[i].resize(image_height - (i * temp));
            for (int j = 0; j < image_height - i * temp; j++)
            {
                imageVerticalIter[i][j] = i * temp + j;
            }
        }
    }

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    // Render 
    int linesLeft = image_height;
    std::for_each(std::execution::par, threadsIter.begin(), threadsIter.end(),
        [&](int t) {
            //std::cout << "t = " << t << std::endl;
            std::for_each(imageVerticalIter[t].begin(), imageVerticalIter[t].end(),
                [&, t](int y) {

                    linesLeft = linesLeft - 1;
                    //std::cout << "y = " << y << std::endl;
                    std::cerr << "\rLines remaining: " << linesLeft << ' ' << std::flush;

                    std::for_each(imageHorizontalIter.begin(), imageHorizontalIter.end(),
                        [&, t, y](int x) {

                            color pixel_color(0, 0, 0);

                            for (int s = 0; s < samples_per_pixel; ++s)
                            {
                                auto u = (x + random_double()) / (image_width - 1);
                                auto v = (y + random_double()) / (image_height - 1);
                                ray r = cam.get_ray(u, v);
                                pixel_color += ray_color(r, world, max_depth);
                            }
                            std::vector<int> pixelC;
                            pixelC.resize(3);

                            pixelC = write_color(std::cout, pixel_color, samples_per_pixel);

                            pixelArray[y][x][0] = pixelC[0];
                            pixelArray[y][x][1] = pixelC[1];
                            pixelArray[y][x][2] = pixelC[2];
                        });
                });
        });

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Duration = " << duration.count() << " seconds." << std::endl;
    
    //Export .bmp image
    image im(image_width, image_height);
    im.Export("image.bmp", pixelArray);

    //SDL
    sdlHelper sdl_help(image_width, image_height);
    sdl_help.Run(pixelArray);

    return 0;
}
