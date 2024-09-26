
#include "RequestBody.hpp"
#include <istream>
#include <sstream>
CRequestBody::CRequestBody() {

}

CRequestBody::~CRequestBody() {

}
void CRequestBody::ParseFormData(const std::string& data, const std::string& boundary) {
    // Split the data by boundary
    std::size_t startPos = 0;
    std::size_t boundaryPos = 0;
    std::string line;

    std::cout <<  std::endl << "request body " << std::endl;
    std::cout << data;

    while ((boundaryPos = data.find(boundary, startPos)) != std::string::npos) {
        // Extract content between boundaries
        std::size_t contentStart = startPos + boundary.length() + 2; // Skip boundary and \r\n
        std::size_t contentEnd = boundaryPos - 2; // Remove \r\n before boundary
        std::string content = data.substr(contentStart, contentEnd - contentStart);

        if (!content.empty()) {
            // Parse headers and content
            std::istringstream contentStream(content);
            std::string contentDisposition;
            std::getline(contentStream, contentDisposition); // First line is Content-Disposition

            // Extract name from Content-Disposition
            std::size_t namePos = content.find("name=\"");
            if (namePos != std::string::npos) {
                namePos += 6;
                std::size_t nameEnd = content.find("\"", namePos);
                std::string name = content.substr(namePos, nameEnd - namePos);

                // Skip two empty lines to get the actual value
                std::string value;
                std::getline(contentStream, line); // Skip empty line
                std::getline(contentStream, value); // The value

                // Output the name-value pair
                std::cout << "Field name: " << name << "\n";
                std::cout << "Field value: " << value << "\n";
            }
        }

        // Move past the boundary
        startPos = boundaryPos + boundary.length();
    }
}