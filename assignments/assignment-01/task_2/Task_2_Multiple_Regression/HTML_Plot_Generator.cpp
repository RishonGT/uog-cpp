#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct DataPoint {
    double x1, x2, y;
};

vector<DataPoint> readCSV(const string& filename) {
    vector<DataPoint> data;
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return data;
    }
    
    // Skip header line
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string x1_str, x2_str, y_str;
        
        if (getline(ss, x1_str, ',') && 
            getline(ss, x2_str, ',') && 
            getline(ss, y_str)) {
            
            DataPoint point;
            point.x1 = stod(x1_str);
            point.x2 = stod(x2_str);
            point.y = stod(y_str);
            data.push_back(point);
        }
    }
    
    file.close();
    cout << "Loaded " << data.size() << " data points from " << filename << endl;
    return data;
}

void generateHTML(const vector<DataPoint>& data, const string& outputFile) {
    // Plane parameters (same as in Data_output.py)
    double m1 = 2.40402;
    double m2 = 3.58494;
    double b = 606.071;
    
    // Find data ranges
    double min_x1 = data[0].x1, max_x1 = data[0].x1;
    double min_x2 = data[0].x2, max_x2 = data[0].x2;
    
    for (const auto& point : data) {
        min_x1 = min(min_x1, point.x1);
        max_x1 = max(max_x1, point.x1);
        min_x2 = min(min_x2, point.x2);
        max_x2 = max(max_x2, point.x2);
    }
    
    ofstream html(outputFile);
    
    html << R"(<!DOCTYPE html>
<html>
<head>
    <title>3D Data Visualization with Regression Plane</title>
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .plot-container { margin: 20px 0; }
        .info { background: #f0f0f0; padding: 10px; border-radius: 5px; margin: 10px 0; }
    </style>
</head>
<body>
    <h1>3D Data Visualization with Regression Plane</h1>
    
    <div class="info">
        <h3>Plane Equation: y = )" << fixed << setprecision(5) << m1 
         << R"(*x1 + )" << m2 << R"(*x2 + )" << b << R"(</h3>
        <p>Data points: )" << data.size() << R"(</p>
    </div>

    <div class="plot-container">
        <div id="plot3d" style="width:100%;height:600px;"></div>
    </div>
    
    <div style="display: flex;">
        <div class="plot-container" style="flex: 1;">
            <div id="plot2d_x1y" style="width:100%;height:400px;"></div>
        </div>
        <div class="plot-container" style="flex: 1;">
            <div id="plot2d_x2y" style="width:100%;height:400px;"></div>
        </div>
    </div>

    <script>
        // Data points
        var x1_data = [)";
    
    // Output x1 data
    for (size_t i = 0; i < data.size(); i++) {
        html << data[i].x1;
        if (i < data.size() - 1) html << ", ";
    }
    
    html << R"(];
        var x2_data = [)";
    
    // Output x2 data
    for (size_t i = 0; i < data.size(); i++) {
        html << data[i].x2;
        if (i < data.size() - 1) html << ", ";
    }
    
    html << R"(];
        var y_data = [)";
    
    // Output y data
    for (size_t i = 0; i < data.size(); i++) {
        html << data[i].y;
        if (i < data.size() - 1) html << ", ";
    }
    
    html << R"(];
        
        // Plane parameters
        var m1 = )" << m1 << R"(;
        var m2 = )" << m2 << R"(;
        var b = )" << b << R"(;
        
        // Data ranges
        var min_x1 = )" << min_x1 << R"(;
        var max_x1 = )" << max_x1 << R"(;
        var min_x2 = )" << min_x2 << R"(;
        var max_x2 = )" << max_x2 << R"(;
        
        // Generate plane surface
        var x1_plane = [];
        var x2_plane = [];
        var y_plane = [];
        
        var steps = 20;
        for (var i = 0; i <= steps; i++) {
            var x1_row = [];
            var x2_row = [];
            var y_row = [];
            for (var j = 0; j <= steps; j++) {
                var x1_val = min_x1 + (max_x1 - min_x1) * i / steps;
                var x2_val = min_x2 + (max_x2 - min_x2) * j / steps;
                var y_val = m1 * x1_val + m2 * x2_val + b;
                
                x1_row.push(x1_val);
                x2_row.push(x2_val);
                y_row.push(y_val);
            }
            x1_plane.push(x1_row);
            x2_plane.push(x2_row);
            y_plane.push(y_row);
        }
        
        // 3D Plot
        var scatter3d = {
            x: x1_data,
            y: x2_data,
            z: y_data,
            mode: 'markers',
            type: 'scatter3d',
            marker: {
                size: 3,
                color: 'blue',
                opacity: 0.8
            },
            name: 'Data Points'
        };
        
        var surface3d = {
            x: x1_plane,
            y: x2_plane,
            z: y_plane,
            type: 'surface',
            colorscale: 'Viridis',
            opacity: 0.6,
            name: 'Regression Plane',
            showscale: false
        };
        
        var layout3d = {
            title: '3D: x1 vs x2 vs y with Regression Plane',
            scene: {
                xaxis: { title: 'x1' },
                yaxis: { title: 'x2' },
                zaxis: { title: 'y' },
                camera: {
                    eye: { x: 1.5, y: 1.5, z: 1.5 }
                }
            },
            margin: { l: 0, r: 0, b: 0, t: 40 }
        };
        
        Plotly.newPlot('plot3d', [scatter3d, surface3d], layout3d);
        
        // 2D Plot: x1 vs y
        var x1_line = [];
        var y_proj_x1_min = [];
        var y_proj_x1_max = [];
        
        for (var i = 0; i <= 200; i++) {
            var x1_val = min_x1 + (max_x1 - min_x1) * i / 200;
            x1_line.push(x1_val);
            y_proj_x1_min.push(m1 * x1_val + m2 * min_x2 + b);
            y_proj_x1_max.push(m1 * x1_val + m2 * max_x2 + b);
        }
        
        var scatter2d_x1 = {
            x: x1_data,
            y: y_data,
            mode: 'markers',
            type: 'scatter',
            marker: {
                size: 4,
                color: 'green',
                opacity: 0.7
            },
            name: 'Data Points'
        };
        
        var plane_envelope_x1 = {
            x: x1_line.concat(x1_line.slice().reverse()),
            y: y_proj_x1_min.concat(y_proj_x1_max.slice().reverse()),
            fill: 'toself',
            fillcolor: 'rgba(255, 165, 0, 0.3)',
            line: { color: 'rgba(255, 165, 0, 0)' },
            name: 'Plane Projection',
            type: 'scatter'
        };
        
        var layout2d_x1 = {
            title: 'x1 vs y with Plane Projection',
            xaxis: { title: 'x1' },
            yaxis: { title: 'y' },
            margin: { l: 60, r: 20, b: 60, t: 40 }
        };
        
        Plotly.newPlot('plot2d_x1y', [plane_envelope_x1, scatter2d_x1], layout2d_x1);
        
        // 2D Plot: x2 vs y
        var x2_line = [];
        var y_proj_x2_min = [];
        var y_proj_x2_max = [];
        
        for (var i = 0; i <= 200; i++) {
            var x2_val = min_x2 + (max_x2 - min_x2) * i / 200;
            x2_line.push(x2_val);
            y_proj_x2_min.push(m1 * min_x1 + m2 * x2_val + b);
            y_proj_x2_max.push(m1 * max_x1 + m2 * x2_val + b);
        }
        
        var scatter2d_x2 = {
            x: x2_data,
            y: y_data,
            mode: 'markers',
            type: 'scatter',
            marker: {
                size: 4,
                color: 'red',
                opacity: 0.7
            },
            name: 'Data Points'
        };
        
        var plane_envelope_x2 = {
            x: x2_line.concat(x2_line.slice().reverse()),
            y: y_proj_x2_min.concat(y_proj_x2_max.slice().reverse()),
            fill: 'toself',
            fillcolor: 'rgba(255, 165, 0, 0.3)',
            line: { color: 'rgba(255, 165, 0, 0)' },
            name: 'Plane Projection',
            type: 'scatter'
        };
        
        var layout2d_x2 = {
            title: 'x2 vs y with Plane Projection',
            xaxis: { title: 'x2' },
            yaxis: { title: 'y' },
            margin: { l: 60, r: 20, b: 60, t: 40 }
        };
        
        Plotly.newPlot('plot2d_x2y', [plane_envelope_x2, scatter2d_x2], layout2d_x2);
    </script>
</body>
</html>)";
    
    html.close();
    cout << "HTML visualization saved to " << outputFile << endl;
}

int main() {
    // Read data from CSV
    vector<DataPoint> data = readCSV("data.csv");
    
    if (data.empty()) {
        cerr << "No data loaded. Please ensure data.csv exists in the current directory." << endl;
        return 1;
    }
    
    // Generate HTML visualization
    generateHTML(data, "data_visualization.html");
    
    cout << "\nVisualization complete!" << endl;
    cout << "Open 'data_visualization.html' in your web browser to view the plots." << endl;
    cout << "\nThe visualization includes:" << endl;
    cout << "- Interactive 3D scatter plot with regression plane" << endl;
    cout << "- 2D projections showing plane envelopes" << endl;
    cout << "- Same plane parameters as your Python script" << endl;
    
    return 0;
}