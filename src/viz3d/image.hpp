#ifndef LIDAR_SFM_IMAGE_HPP
#define LIDAR_SFM_IMAGE_HPP

#include <stdexcept>
#include <Eigen/Dense>


#include "stb_image_write.h"


namespace viz {

    struct RGB {
        unsigned char r = 0, g = 0, b = 0;

        RGB() = default;

        explicit RGB(int rgba);

        static constexpr short NumValues() { return 3; }

        static constexpr int STB_PNG_COMP() { return 3; }

        unsigned char &operator[](short i);

        const unsigned char &operator[](short i) const;

    };

    struct RGBA {
        unsigned char r = 0, g = 0, b = 0, a = 0;

        RGBA() = default;

        static constexpr short NumValues() { return 4; }

        static constexpr int STB_PNG_COMP() { return 4; }

        unsigned char &operator[](short i);

        const unsigned char &operator[](short i) const;


        explicit RGBA(int rgba);
    };

#define OPERATOR_DEFINITION(DATA_TYPE, OP) \
DATA_TYPE operator OP (const DATA_TYPE lhs, const DATA_TYPE rhs);


#define OPERATORS_DEFINITION(DATA_TYPE) \
OPERATOR_DEFINITION(DATA_TYPE, +)       \
OPERATOR_DEFINITION(DATA_TYPE, -)       \
OPERATOR_DEFINITION(DATA_TYPE, *)       \
OPERATOR_DEFINITION(DATA_TYPE, /)


    OPERATORS_DEFINITION(RGB);

    OPERATORS_DEFINITION(RGBA);

    template<typename T>
    class Image {
    public:
        typedef Eigen::Matrix<T, -1, -1, Eigen::RowMajor> ImageT;

        // Clears the Image and set all pixels to Zero
        void EmptyImage(int height, int width) {
            image_data_ = ImageT::Zero(height, width);
        };

        inline int Height() const { return image_data_.rows(); };

        inline int Width() const { return image_data_.cols(); };

        ImageT &ImageData() { return image_data_; };

        const ImageT &ImageData() const { return image_data_; };

        // Writes the image to disk
        bool WriteImage(const std::string &filepath) const {
            stbi_write_png(filepath.c_str(), Width(), Height(), 3,
                           (const void *) &image_data_(0, 0), Width() * sizeof(RGB));
            return true;
        };

        // Loads the Image from Disk
        // TODO - Support Multiple formats
        bool LoadImage(const std::string &filepath) {
            return false;
        };

    private:
        Eigen::Matrix<T, -1, -1, Eigen::RowMajor> image_data_;
    };


} // namespace viz


#endif //LIDAR_SFM_IMAGE_HPP
