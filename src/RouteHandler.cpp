
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "DispatchTask.hpp"
#include "RouteHandler.hpp"

void CRouteHandler::AddRoute(const std::string& path, CRouteCallback callback) {
    auto segments = splitPath(path);

    Node* currentNode = &root_;

    // Traverse the tree, adding nodes as necessary
    for (const auto& segment : segments) {
        if (currentNode->children.find(segment) == currentNode->children.end()) {
            currentNode->children[segment] = Node();
        }
        currentNode = &currentNode->children[segment];
    }

    // At the leaf node, store the callback for the specific HTTP method
    currentNode->callbacks[callback.type] = callback;

}


std::optional<CRouteCallback> CRouteHandler::FindRoute(const Node& node, const std::vector<std::string>& segments,  EHttpRequestType requestType) {
    Node* currentNode = &root_;
    for (const auto& segment : segments) {
        if (currentNode->children.find(segment) == currentNode->children.end())
            return {};
        currentNode = &currentNode->children[segment];
    }

    auto it = currentNode->callbacks.find(requestType);
    if (it != currentNode->callbacks.end())
        return it->second;

    return {};
}
std::vector<std::string> CRouteHandler::splitPath(const std::string& path)
{


    // Split the path by '/'
    std::vector<std::string> segments;
    size_t start = 0, end = 0;
    if (path.at(0) == '/')
        start = 1;

    while ((end = path.find('/', start)) != std::string::npos) {
        segments.push_back(path.substr(start, end - start));
        start = end + 1;
    }
    segments.push_back(path.substr(start));
    return segments;
}

std::optional<CRouteCallback> CRouteHandler::GetRoute(const std::string& path, EHttpRequestType requestType) {
    auto segments = splitPath(path);
    return FindRoute(root_, segments, requestType);
}
