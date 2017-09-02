#include "dataloader.h"
#include "dataHandling/CSVRow.h"
#include "MyVectors.h"

using namespace std;

DataLoader::DataLoader()
{

}

DataLoader::~DataLoader()
{
	for (size_t i = 0; i < _pointData.size(); i++)
	{
		_pointData[i].clear();
	}
	_pointData.clear();
}


bool DataLoader::loadCSVtoPointCloud(const QString &fileName)
{
// Load comma seperated file (CSV)
    _ifData.open(fileName.toStdWString().c_str());
    if (!_ifData.good())
        return false;

    _pointData.clear();
//    string fileExt = SysTools::ToLowerCase(SysTools::GetExt(fileName.toStdString().c_str()));
    CSVRow   row;
    cout << "Try comma as delimiter." << endl;
    row = CSVRow(',');

    //getNextLineAndSplitIntoTokens
    bool firstTime = true;
    vector<FLOATVECTOR2> attrMinMax;
    while (_ifData>>row)
    {
        vector<float> fVal_row(row.size());
        if (firstTime){
            if (row.size() == 1)
            {
                // If it turns out that semi-colon is used, switch delimiter to semi-colon
                cout << "Try semi-colon as delimiter." << endl;
                row = CSVRow(';');
                // Roll back to the beginning of the file
                _ifData.clear();
                _ifData.seekg(0, ios::beg);
                continue;
            }
            else
            {
                attrMinMax.resize(row.size(), FLOATVECTOR2(numeric_limits<float>::max(), -numeric_limits<float>::max()));
                _attrib_names.resize(row.size());
                for (size_t i = 0; i < _attrib_names.size(); i++)
                    _attrib_names[i] = "X " + number2String(int(i));
                bool isDigit = true;
                for (size_t i = 0; i < row.size(); i++)
                {
                    if (atoi(row[i].c_str()) == 0)
                    {
                        isDigit = isdigit(row[i].c_str()[0]);
                    }
                    if (!isDigit)
                        _attrib_names[i] = row[i];
                    else
                    {
                        fVal_row[i] = atof(row[i].c_str());
                        attrMinMax[i].x = MIN(fVal_row[i], attrMinMax[i].x); // Get minmax
                        attrMinMax[i].y = MAX(fVal_row[i], attrMinMax[i].y);
                    }

                }
                if (isDigit)
                    // Add to pointData
                    _pointData.push_back(fVal_row);
                firstTime = false;
            }
        }
		else{

			if (fVal_row.size() != attrMinMax.size())
				continue; // don't add to data if row sizes are not consistent

			for (size_t i = 0; i < row.size(); i++)
			{
				fVal_row[i] = atof(row[i].c_str()); // For now, don't handle nominal data, just use as float!
				attrMinMax[i].x = MIN(fVal_row[i], attrMinMax[i].x); // Get minmax
				attrMinMax[i].y = MAX(fVal_row[i], attrMinMax[i].y);
			}

			// Add to pointData
			_pointData.push_back(fVal_row);
		}
    }
    return true;
}

std::vector<std::vector<float> > DataLoader::pointData() const
{
    return _pointData;
}

void DataLoader::setPointData(const std::vector<std::vector<float> > &pointData)
{
    _pointData = pointData;
}

std::vector<std::string> DataLoader::attrib_names() const
{
    return _attrib_names;
}

void DataLoader::setAttrib_names(const std::vector<std::string> &attrib_names)
{
    _attrib_names = attrib_names;
}
