#include "Mat.h"

Mat::V2f::V2f()
{
	h = 0;
	w = 0;
}

Mat::V2f::V2f(int height, int width)
{

	h = height;
	w = width;

	matrix = std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f));
}

Mat::V2f::V2f(std::vector<std::vector<float>> construct)
{
	if (construct.size() != h)
	{
		throw MATRIX_ERROR("Error constructing V2f, constructor size not equal to matrix size");
	}

	matrix = construct;
}

std::vector<float>& Mat::V2f::operator[](int pos)
{
	return matrix[pos];
}

Mat::V2f Mat::V2f::operator+(V2f mat2)
{
	if (mat2.height() != this->height() or mat2.width() != this->width())
	{
		throw MATRIX_ERROR("Error adding matrices, sizes are unequal");
	}


	V2f returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] + mat2[i][j];
		}
	}

	return returnMatrix;
}

Mat::V2f Mat::V2f::operator+(float val)
{
	V2f returnMatrix(this->height(), this->width());

	for (int i = 0; i < returnMatrix.height(); i++)
	{
		for (int j = 0; j < returnMatrix.width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] + val;
		}
	}

	return returnMatrix;
}

Mat::V2f Mat::V2f::operator-(V2f mat2)
{
	if (mat2.height() != this->height() or mat2.width() != this->width())
	{
		throw MATRIX_ERROR("Error subtracting matrices, sizes are unequal");
	}


	V2f returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] - mat2[i][j];
		}
	}

	return returnMatrix;
}

Mat::V2f Mat::V2f::operator-(float val)
{
	V2f returnMatrix(this->height(), this->width());

	for (int i = 0; i < returnMatrix.height(); i++)
	{
		for (int j = 0; j < returnMatrix.width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] - val;
		}
	}

	return returnMatrix;
}

Mat::V2f Mat::V2f::operator*(V2f mat2)
{
	if (this->width() != mat2.height())
	{
		throw MATRIX_ERROR("Error multiplying matrices, width of first must equal height of second");
	}

	V2f returnMatrix(this->height(), mat2.width());

	for (int y1 = 0; y1 < this->height(); y1++)
	{
		for (int x2 = 0; x2 < mat2.width(); x2++)
		{
			for (int i = 0; i < this->width(); i++)
			{
				returnMatrix[y1][x2] += matrix[y1][i] * mat2[i][x2];
			}
		}
	}

	return returnMatrix;
}

Mat::V2f Mat::V2f::operator*(float val)
{
	V2f returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] * val;
		}
	}

	return returnMatrix;
}

Mat::V2f Mat::V2f::transpose()
{
	V2f tempMatrix(this->width(), this->height());

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			tempMatrix[j][i] = matrix[i][j];
		}
	}

	return tempMatrix;
}

std::vector<float> Mat::V2f::sumRows()
{
	std::vector<float> returnVector(this->width(), 0.0f);

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnVector[i] += matrix[i][j];
		}
	}

	return returnVector;
}

std::vector<float> Mat::V2f::sumCols()
{
	std::vector<float> returnVector(this->height(), 0.0f);

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnVector[j] += matrix[i][j];
		}
	}

	return returnVector;
}

int Mat::V2f::height()
{
	return h;
}

int Mat::V2f::width()
{
	return w;
}

void Mat::V2f::ex()
{
	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			matrix[i][j] = exp(matrix[i][j]);
		}
	}
}

void Mat::V2f::printMatrix()
{
	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

Mat::V2i::V2i()
{
	h = 0;
	w = 0;
}

Mat::V2i::V2i(int height, int width)
{

	h = height;
	w = width;

	matrix = std::vector<std::vector<int>>(height, std::vector<int>(width, 0.0f));

}

Mat::V2i::V2i(std::vector<std::vector<int>> construct)
{
	if (construct.size() != h)
	{
		throw MATRIX_ERROR("Error constructing V2i, constructor size not equal to matrix size");
	}

	matrix = construct;
}

std::vector<int>& Mat::V2i::operator[](int pos)
{
	return matrix[pos];
}

Mat::V2i Mat::V2i::operator+(V2i mat2)
{
	if (mat2.height() != this->height() or mat2.width() != this->width())
	{
		throw MATRIX_ERROR("Error adding matrices, sizes are unequal");
	}


	V2i returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] + mat2[i][j];
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::V2i::operator+(int val)
{
	V2i returnMatrix(this->height(), this->width());

	for (int i = 0; i < returnMatrix.height(); i++)
	{
		for (int j = 0; j < returnMatrix.width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] + val;
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::V2i::operator-(V2i mat2)
{
	if (mat2.height() != this->height() or mat2.width() != this->width())
	{
		throw MATRIX_ERROR("Error subtracting matrices, sizes are unequal");
	}


	V2i returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] - mat2[i][j];
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::V2i::operator-(int val)
{
	V2i returnMatrix(this->height(), this->width());

	for (int i = 0; i < returnMatrix.height(); i++)
	{
		for (int j = 0; j < returnMatrix.width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] - val;
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::V2i::operator*(V2i mat2)
{
	if (this->width() != mat2.height())
	{
		throw MATRIX_ERROR("Error multiplying matrices, width of first must equal height of second");
	}

	V2i returnMatrix(this->height(), mat2.width());

	for (int y1 = 0; y1 < this->height(); y1++)
	{
		for (int x2 = 0; x2 < mat2.width(); x2++)
		{
			for (int i = 0; i < this->width(); i++)
			{
				returnMatrix[y1][x2] += matrix[y1][i] * mat2[i][x2];
			}
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::V2i::operator*(int val)
{
	V2i returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] * val;
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::V2i::transpose()
{
	V2i tempMatrix(this->width(), this->height());

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			tempMatrix[j][i] = matrix[i][j];
		}
	}

	return tempMatrix;
}

std::vector<int> Mat::V2i::sumRows()
{
	std::vector<int> returnVector(this->width(), 0.0f);

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnVector[i] += matrix[i][j];
		}
	}

	return returnVector;
}

std::vector<int> Mat::V2i::sumCols()
{
	std::vector<int> returnVector(this->height(), 0.0f);

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnVector[j] += matrix[i][j];
		}
	}

	return returnVector;
}

int Mat::V2i::height()
{
	return h;
}

int Mat::V2i::width()
{
	return w;
}

void Mat::V2i::ex()
{
	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			matrix[i][j] = exp(matrix[i][j]);
		}
	}
}

void Mat::V2i::printMatrix()
{
	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

Mat::V2d::V2d()
{
	h = 0;
	w = 0;
}

Mat::V2d::V2d(int height, int width)
{

	h = height;
	w = width;

	matrix = std::vector<std::vector<double>>(height, std::vector<double>(width, 0.0f));
}

Mat::V2d::V2d(std::vector<std::vector<double>> construct)
{
	if (construct.size() != h)
	{
		throw MATRIX_ERROR("Error constructing V2d, constructor size not equal to matrix size");
	}

	matrix = construct;
}

std::vector<double>& Mat::V2d::operator[](int pos)
{
	return matrix[pos];
}

Mat::V2d Mat::V2d::operator+(V2d mat2)
{
	if (mat2.height() != this->height() or mat2.width() != this->width())
	{
		throw MATRIX_ERROR("Error adding matrices, sizes are unequal");
	}


	V2d returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] + mat2[i][j];
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::V2d::operator+(double val)
{
	V2d returnMatrix(this->height(), this->width());

	for (int i = 0; i < returnMatrix.height(); i++)
	{
		for (int j = 0; j < returnMatrix.width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] + val;
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::V2d::operator-(V2d mat2)
{
	if (mat2.height() != this->height() or mat2.width() != this->width())
	{
		throw MATRIX_ERROR("Error subtracting matrices, sizes are unequal");
	}


	V2d returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] - mat2[i][j];
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::V2d::operator-(double val)
{
	V2d returnMatrix(this->height(), this->width());

	for (int i = 0; i < returnMatrix.height(); i++)
	{
		for (int j = 0; j < returnMatrix.width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] - val;
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::V2d::operator*(V2d mat2)
{
	if (this->width() != mat2.height())
	{
		throw MATRIX_ERROR("Error multiplying matrices, width of first must equal height of second");
	}

	V2d returnMatrix(this->height(), mat2.width());

	for (int y1 = 0; y1 < this->height(); y1++)
	{
		for (int x2 = 0; x2 < mat2.width(); x2++)
		{
			for (int i = 0; i < this->width(); i++)
			{
				returnMatrix[y1][x2] += matrix[y1][i] * mat2[i][x2];
			}
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::V2d::operator*(double val)
{
	V2d returnMatrix(this->height(), this->width());


	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnMatrix[i][j] = matrix[i][j] * val;
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::V2d::transpose()
{
	V2d tempMatrix(this->width(), this->height());

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			tempMatrix[j][i] = matrix[i][j];
		}
	}

	return tempMatrix;
}

std::vector<double> Mat::V2d::sumRows()
{
	std::vector<double> returnVector(this->width(), 0.0f);

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnVector[i] += matrix[i][j];
		}
	}

	return returnVector;
}

std::vector<double> Mat::V2d::sumCols()
{
	std::vector<double> returnVector(this->height(), 0.0f);

	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			returnVector[j] += matrix[i][j];
		}
	}

	return returnVector;
}

int Mat::V2d::height()
{
	return h;
}

int Mat::V2d::width()
{
	return w;
}

void Mat::V2d::ex()
{
	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			matrix[i][j] = exp(matrix[i][j]);
		}
	}
}

void Mat::V2d::printMatrix()
{
	for (int i = 0; i < this->height(); i++)
	{
		for (int j = 0; j < this->width(); j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

Mat::V2f Mat::addOuterf(std::vector<float> v1, std::vector<float> v2)
{
	Mat::V2f returnMatrix(v1.size(), v2.size());

	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			returnMatrix[i][j] = v1[i] + v2[j];
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::addOuteri(std::vector<int> v1, std::vector<int> v2)
{
	Mat::V2i returnMatrix(v1.size(), v2.size());

	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			returnMatrix[i][j] = v1[i] + v2[j];
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::addOuterd(std::vector<double> v1, std::vector<double> v2)
{
	Mat::V2d returnMatrix(v1.size(), v2.size());

	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			returnMatrix[i][j] = v1[i] + v2[j];
		}
	}

	return returnMatrix;
}