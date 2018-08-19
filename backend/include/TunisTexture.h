#ifndef TUNISTEXTURE_H
#define TUNISTEXTURE_H

#include <cinttypes>

namespace tunis
{

class Texture
{
public:

    enum Filtering
    {
        FilterNone, FilterBilinear, FilterBilinearMipmap, FilterTrilinear
    };

    Texture();
    Texture(int width, int height, Filtering filtering = FilterTrilinear);
    Texture(const Texture &other);
    ~Texture();

    Texture &operator=(const Texture &other);

    void bind();

    inline size_t getId() const
    {
        return id;
    }

private:

    size_t id;

};


}

inline bool operator==(const tunis::Texture &lhs, const tunis::Texture &rhs)
{
    return lhs.getId() == rhs.getId();
}

inline bool operator!=(const tunis::Texture &lhs, const tunis::Texture &rhs)
{
    return lhs.getId() != rhs.getId();
}


#endif // TUNISTEXTURE_H
