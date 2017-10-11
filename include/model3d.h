#ifndef MODEL_H
#define MODEL_H

class QString;
class Mesh;

class Model3d
{
public:
	virtual ~Model3d() {}

	virtual void loadFromFile(const QString& fileName) = 0;
	virtual Mesh* mesh() const = 0;
};

#endif // MODEL_H
