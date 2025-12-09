
#include <string>
#include <vector>
#include <fstream>

enum class ErrorCode
{
	SUCCESS_TO_LOAD,
	FAILED_TO_LOAD
};

enum class MarkerType
{
	SOI,
	EOI,
	SOS,
	APP0,
	APP1,
	DQT,
	SOF0,
	DHT,
	UNKNOWN
};

enum class RepairStatus
{
	NOT_REPAIRED,
	REPAIRED_SUCCESSFULLY,
	REPAIR_FAILED
};

enum class MarkerAnalyseResult
{
	MARKER_IS_OK,
	MARKER_IS_CORRUPTED
};

class JPGManager {

	public:
	JPGManager();

	~JPGManager();

	ErrorCode loadImage(const std::string& filePath);

	bool saveImage(const std::string& filePath);

	void repairImage();

	void displayImageInfo() const;

	void analyzeMarkers();

private:

	std::string imagePath;

	std::ifstream in;

	std::vector<unsigned char> imageData;

	bool isImageLoaded;

	void insertEOIMarker();

	// Markers
	unsigned short marker;

	MarkerAnalyseResult analyzeBeginningMarkers();

	MarkerAnalyseResult analyzeEndMarkers();
};