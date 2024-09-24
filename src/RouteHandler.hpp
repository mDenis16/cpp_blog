#pragma once
#include <functional>
#include <unordered_map>
#include <iostream>
#include <optional>


struct CRouteCallback {
public:
    EHttpRequestType type;
    std::function<std::variant<CHttpResponse, std::function<CHttpResponse()>>(std::shared_ptr<CHttpRequest>& request)> func;
};

class CRouteHandler {
public:

    void AddRoute(const std::string& path, CRouteCallback callback);


    std::optional<CRouteCallback> GetRoute(const std::string& path) ;

private:
    struct Node {
        std::unordered_map<std::string, Node> children;
        CRouteCallback callback;
    };

    Node root_;

    void AddRouteRecursively(Node& node, const std::vector<std::string>& segments, CRouteCallback callback);
    bool FindRoute(const Node& node, const std::vector<std::string>& segments, CRouteCallback& callback);

    std::vector<std::string> splitPath(const std::string& path);
};
