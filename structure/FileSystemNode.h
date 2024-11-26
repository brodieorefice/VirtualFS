#ifndef FILESYSTEMNODE_H
#define FILESYSTEMNODE_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
class FileSystemNode {
public:
    std::string name;
    bool is_directory;
    std::vector<FileSystemNode*> children; // For directories
    std::string content; // For files

    FileSystemNode(std::string name, bool is_directory) 
        : name(std::move(name)), is_directory(is_directory), content("") {}

    void add_child(FileSystemNode* child) {
        if (!is_directory) {
            throw std::invalid_argument(name + " is not a directory!");
        }
        children.push_back(child);
    }
};

class FileSystem {
private:
    FileSystemNode* root;

    FileSystemNode* traverse(const std::string& path) {
        FileSystemNode* current = root;
        if (path == "/") return current;

        size_t start = 1; // Skip initial "/"
        while (start < path.size()) {
            size_t end = path.find('/', start);
            std::string part = path.substr(start, end - start);
            bool found = false;

            for (auto* child : current->children) {
                if (child->name == part) {
                    current = child;
                    found = true;
                    break;
                }
            }

            if (!found) {
                throw std::invalid_argument("Path " + path + " not found!");
            }

            if (end == std::string::npos) break;
            start = end + 1;
        }
        return current;
    }

public:
    FileSystem() {
        root = new FileSystemNode("/", true);
    }

    ~FileSystem() {
        // Simple cleanup for all nodes; use smart pointers for better memory management in production.
        delete root;
    }

    void add(const std::string& path, const std::string& name, bool is_directory) {
        FileSystemNode* current = traverse(path);
        FileSystemNode* new_node = new FileSystemNode(name, is_directory);
        current->add_child(new_node);
    }

    void write_to_file(const std::string& path, const std::string& content) {
        FileSystemNode* file_node = traverse(path);
        if (file_node->is_directory) {
            throw std::invalid_argument(path + " is a directory! Cannot add content.");
        }
        file_node->content += content; // Append to the existing content
    }

    std::string read_file(const std::string& path) {
        FileSystemNode* file_node = traverse(path);
        if (file_node->is_directory) {
            throw std::invalid_argument(path + " is a directory! Cannot read content.");
        }
        return file_node->content;
    }

    void display(FileSystemNode* node = nullptr, int indent = 0) {
        if (node == nullptr) node = root;

        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << node->name << "\n";

        if (node->is_directory) {
            for (auto* child : node->children) {
                display(child, indent + 1);
            }
        }
    }
};

    /*

    FileSystem fs;

    fs.add("/", "home", true);
    fs.add("/home", "user1", true);
    fs.add("/home/user1", "file1.txt", false);
    fs.add("/", "etc", true);
    fs.add("/etc", "config", false);

    // Write content to a file
    fs.write_to_file("/home/user1/file1.txt", "Hello, World!\n");
    fs.write_to_file("/home/user1/file1.txt", "This is the second line.\n");
    fs.write_to_file("/etc/config", "Configuration data here.");

    // Read content from a file
    std::cout << "Contents of /home/user1/file1.txt:\n";
    std::cout << fs.read_file("/home/user1/file1.txt") << "\n";

    std::cout << "\nContents of /etc/config:\n";
    std::cout << fs.read_file("/etc/config") << "\n";

    // Display the file system structure
    std::cout << "\nFile System Structure:\n";
    fs.display();
    */
    

#endif // FILESYSTEMNODE_H