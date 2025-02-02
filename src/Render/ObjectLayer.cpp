#include <Render/ObjectLayer.h>

ObjectLayer::ObjectLayer(const tmx::Map& map, std::size_t layerID)
{
    // Get the layer from the map using the provided ID
    const auto& layers = map.getLayers();
    if (layerID >= layers.size() || layers[layerID]->getType() != tmx::Layer::Type::Object)
    {
        throw std::runtime_error("Invalid layer ID or layer type.");
    }

    const auto& objectGroup = dynamic_cast<const tmx::ObjectGroup&>(*layers[layerID]);
    for (const auto& object : objectGroup.getObjects())
    {
        if (auto shape = createShape(object))
        {
            m_shapes.emplace_back(shape);
        }
    }
}

sf::Shape* ObjectLayer::createShape(const tmx::Object& object)
{
    sf::Shape* shape = nullptr;

    // Handle different object types
    switch (object.getShape())
    {
    case tmx::Object::Shape::Point:
    {
        sf::Vector2f position(object.getPosition().x, object.getPosition().y);

        std::unordered_map<std::string, std::string> properties;
        for (const auto& property : object.getProperties())
        {
            properties[property.getName()] = property.getStringValue();
        }

        m_points.emplace_back(position, properties);
        break;
    }
    case tmx::Object::Shape::Rectangle:
        shape = new sf::RectangleShape({ object.getAABB().width, object.getAABB().height });
        shape->setPosition(sf::Vector2f(object.getPosition().x, object.getPosition().y));
        break;

    case tmx::Object::Shape::Ellipse:
        // Create a circle shape approximating an ellipse
        shape = new sf::CircleShape(object.getAABB().width / 2.f);
        shape->setScale(1.f, object.getAABB().height / object.getAABB().width);
        shape->setPosition(sf::Vector2f(object.getPosition().x, object.getPosition().y));
        break;

    case tmx::Object::Shape::Polygon:
    case tmx::Object::Shape::Polyline:
    {
        auto& points = object.getPoints();
        auto convexShape = new sf::ConvexShape(points.size());
        for (std::size_t i = 0; i < points.size(); ++i)
        {
            convexShape->setPoint(i, { points[i].x, points[i].y });
        }
        convexShape->setPosition(sf::Vector2f(object.getPosition().x, object.getPosition().y));
        shape = convexShape;
        break;
    }

    default:
        // Unsupported object shape
        break;
    }

    if (shape)
    {
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineColor(sf::Color::Red);
        shape->setOutlineThickness(1.f);
    }

    return shape;
}

void ObjectLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Apply the transform
    states.transform *= getTransform();

    // Draw each shape
    for (const auto& shape : m_shapes)
    {
        target.draw(*shape, states);
    }

    for (const auto& point : m_points)
    {
        sf::CircleShape pointVisual(3.f); // Radio del punto visual
        pointVisual.setFillColor(sf::Color::Green);
        pointVisual.setPosition(point.first.x - pointVisual.getRadius(), point.first.y - pointVisual.getRadius());
        target.draw(pointVisual, states);
    }
}