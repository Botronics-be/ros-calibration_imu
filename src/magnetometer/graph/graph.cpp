#include "graph.h"

using namespace magnetometer;

graph::graph(std::shared_ptr<magnetometer::data_interface>& data_interface)
{
    // Store data interface.
    graph::m_data_interface = data_interface;

    // Initialize scatter instance.
    graph::m_graph = new QtDataVisualization::Q3DScatter();
    graph::m_graph->axisX()->setTitle("x");
    graph::m_graph->axisX()->setTitleVisible(true);
    graph::m_graph->axisY()->setTitle("z");
    graph::m_graph->axisY()->setTitleVisible(true);
    graph::m_graph->axisZ()->setTitle("y");
    graph::m_graph->axisZ()->setTitleVisible(true);
    graph::m_graph->setAspectRatio(1.0);
    graph::m_graph->setHorizontalAspectRatio(1.0);
    graph::m_graph->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQuality::ShadowQualityNone);

    // Initialize uncalibrated series.
    graph::m_series_uncalibrated = new QtDataVisualization::QScatter3DSeries();
    graph::m_series_uncalibrated->setBaseColor(QColor(Qt::GlobalColor::blue));
    graph::m_series_uncalibrated->setItemSize(0.1);
    graph::m_graph->addSeries(graph::m_series_uncalibrated);

    // Initialize uncalibrated newest series.
    graph::m_series_uncalibrated_new = new QtDataVisualization::QScatter3DSeries();
    graph::m_series_uncalibrated_new->setBaseColor(QColor(Qt::GlobalColor::red));
    graph::m_series_uncalibrated_new->setItemSize(0.5);
    graph::m_graph->addSeries(graph::m_series_uncalibrated_new);

    // Initialize calibrated series.
    graph::m_series_calibrated = new QtDataVisualization::QScatter3DSeries();
    graph::m_series_calibrated->setBaseColor(QColor(Qt::GlobalColor::green));
    graph::m_series_calibrated->setItemSize(0.1);
    graph::m_graph->addSeries(graph::m_series_calibrated);

    // Initialize flags.
    graph::f_indicate_new_point = true;
}

graph::~graph()
{
    // Clean up pointers.
    delete graph::m_graph;
}

QWidget* graph::get_widget()
{
    return QWidget::createWindowContainer(graph::m_graph);
}


void graph::uncalibrated_visible(bool visible)
{
    graph::m_series_uncalibrated->setVisible(visible);
    graph::m_series_uncalibrated_new->setVisible(visible);
}
void graph::indicate_new_point(bool enabled)
{
    bool update_series = (enabled != graph::f_indicate_new_point);
    graph::f_indicate_new_point = enabled;

    if(update_series)
    {
        graph::update_uncalibrated_plot();
    }
}
void graph::calibrated_visible(bool visible)
{
    graph::m_series_calibrated->setVisible(visible);
}

void graph::update_uncalibrated_plot()
{
    // Add all uncalibrated data points to the series.

    // Get total number of points.
    uint32_t n_points = graph::m_data_interface->n_points();

    // Create new data arrays.
    QtDataVisualization::QScatterDataArray* uncalibrated_points = new QtDataVisualization::QScatterDataArray();
    QtDataVisualization::QScatterDataArray* new_point = new QtDataVisualization::QScatterDataArray();

    // Reserve space in the uncalibrated points.
    uncalibrated_points->reserve(n_points);

    // Populate uncalibrated points array.
    for(uint32_t i = 0; i < n_points; ++i)
    {
        QVector3D point;
        graph::m_data_interface->get_point(i, point);
        uncalibrated_points->append(point);
    }

    // Set up newest point if enabled.
    if(graph::f_indicate_new_point && n_points > 0)
    {
        new_point->append(uncalibrated_points->takeLast());
    }

    // Add to series.
    graph::m_series_uncalibrated->dataProxy()->resetArray(uncalibrated_points);
    graph::m_series_uncalibrated_new->dataProxy()->resetArray(new_point);
}
void graph::update_calibrated_plot()
{

}
