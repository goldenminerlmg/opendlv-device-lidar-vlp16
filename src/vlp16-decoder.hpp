/*
 * opendlv-device-lidar-vlp16 decodes VLP16 data realized for OpenDLV.
 * Copyright (C) 2018  Christian Berger
 * Copyright (C) 2017  Hang Yin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VLP16_DECODER
#define VLP16_DECODER

#include "opendlv-standard-message-set.hpp"

#include <cstdint>
#include <array>
#include <sstream>
#include <vector>

class VLP16Decoder {
   private:
    VLP16Decoder(const VLP16Decoder &) = delete;
    VLP16Decoder(VLP16Decoder &&)      = delete;
    VLP16Decoder &operator=(const VLP16Decoder &) = delete;
    VLP16Decoder &operator=(VLP16Decoder &&) = delete;

   public:
    VLP16Decoder() noexcept;
    ~VLP16Decoder() = default;

   public:
    /**
     * This method decodes the next packet from VLP16.
     *
     * @return PointCloud.
     */
    opendlv::proxy::PointCloudReading decode(const std::string &data) noexcept;

   private:
    void setupCalibration() noexcept;
    void index16sensorIDs() noexcept;

   private:
    const uint32_t MAX_POINT_SIZE{30000}; // The maximum number of points per frame. 
    uint8_t m_distanceEncoding{0}; // 0: cm; 1: 2mm. For now, always 0 = cm.

    std::array<float, 16> m_verticalAngle{};
    std::array<uint8_t, 16> m_sensorOrderIndex{}; // Specify the sensor ID order for each 16 points with increasing vertical angle for PointCloudReading.
    std::array<uint16_t, 16> m_16SensorsNoIntensity{}; // Store the distance values of the current 16 sensors for PointCloudReading without intensities.

    float m_startAzimuth{0.0f};
    float m_currentAzimuth{0.0f};
    float m_previousAzimuth{0.0f};
    float m_nextAzimuth{0.0f};
    float m_deltaAzimuth{0.0f};
    uint32_t m_pointIndexCPC{0}; // Current number of points of the current frame for compact point cloud.

    std::stringstream m_distanceStringStreamNoIntensity{};
};

#endif
