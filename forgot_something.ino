#include <Wire.h>
#include <ArduCAM.h>
#include <opencv2/opencv.hpp>

ArduCAM myCAM(OV2640, CS);


cv::Scalar color_mug(0, 0, 255);
int color_tolerance = 10;

void setup()
{
    myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK); // VSYNC is active LOW
    myCAM.write_reg(ARDUCHIP_GPIO, GP_PWDN_MASK);
    myCAM.OV2640_set_JPEG_size(OV2640_800x600);
    myCAM.init();
}

void loop()
{
    std::pair<cv::Rect, bool> red_mug = find_mug(color_mug, color_tolerance);
}

// function to find a mug I likely forgot in the kitchen again
std::pair<cv::Rect, bool> find_mug(cv::Scalar color_mug, int color_tolerance)
{
    if (myCAM.capture())
    {
        myCAM.flush_fifo();
        myCAM.clear_fifo_flag();
        int imageSize = myCAM.read_fifo_length();
        if (imageSize >= MAX_FIFO_SIZE)
        {
            Serial.println("Overflow!");
            return std::make_pair(cv::Rect(), false);
        }
        uint8_t *buf = (uint8_t *)malloc(imageSize);
        myCAM.read_fifo(buf, imageSize);
        cv::Mat image = cv::imdecode(cv::Mat(1, imageSize, CV_8UC3, buf), cv::IMREAD_COLOR);
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        std::vector<std::vector<cv::Point>> squares;
        cv::findContours(image, squares, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < squares.size(); i++)
        {
            cv::Rect rect = cv::boundingRect(squares[i]);
            cv::Mat squareROI = image(rect);
            cv::inRange(squareROI, color - tolerance, color + tolerance, squareROI);
            if (cv::countNonZero(squareROI) > 0)
            {
                float aspect_ratio = rect.width / rect.height;
                if (0.95 < aspect_ratio && aspect_ratio < 1.05)
                {
                    Serial.println("Found a square at (x: " + String(rect.x) + ", y: " + String(rect.y) + ") with width " + String(rect.width) + ", and height " + String(rect.height));
                    free(buf);
                    return std::make_pair
                }
            }
            free(buf);
        }
        return std::make_pair(cv::Rect(), false);
    }
}