#ifndef SQLiteProcedures_hh_INCLUDED
#define SQLiteProcedures_hh_INCLUDED

// ObjexxFCL Headers
#include <ObjexxFCL/FArray1D.hh>
#include <ObjexxFCL/FArray2D.hh>
#include <ObjexxFCL/Optional.hh>

// EnergyPlus Headers
#include <EnergyPlus.hh>

#include <sqlite3.h>

#include <fstream>
#include <memory>

namespace EnergyPlus {

class SQLite {
public:

	// Open the DB and prepare for writing data
	// Create all of the tables on construction
	SQLite();

	// Close database and free prepared statements
	virtual ~SQLite();

	bool writeOutputToSQLite() const;
	bool writeTabularDataToSQLite() const;

	// Begin a transaction
	void sqliteBegin();

	// Commit a transaction
	void sqliteCommit();

	void createSQLiteReportVariableDictionaryRecord(
		int const reportVariableReportID,
		int const storeTypeIndex,
		std::string const & indexGroup,
		std::string const & keyedValueString,
		std::string const & variableName,
		int const indexType,
		std::string const & units,
		int const reportingFreq,
		Optional_string_const ScheduleName = _
	);

	void createSQLiteReportVariableDataRecord(
		int const recordIndex,
		Real64 const value,
		Optional_int_const reportingInterval = _,
		Optional< Real64 const > minValue = _,
		Optional_int_const minValueDate = _,
		Optional< Real64 const > maxValue = _,
		Optional_int_const maxValueDate = _,
		Optional_int_const minutesPerTimeStep = _
	);

	void createSQLiteTimeIndexRecord(
		int const reportingInterval,
		int const recordIndex,
		int const CumlativeSimulationDays,
		Optional_int_const Month = _,
		Optional_int_const DayOfMonth = _,
		Optional_int_const Hour = _,
		Optional< Real64 const > EndMinute = _,
		Optional< Real64 const > StartMinute = _,
		Optional_int_const DST = _,
		Optional_string_const DayType = _
	);

	void addSQLiteZoneSizingRecord(
		std::string const & ZoneName, // the name of the zone
		std::string const & LoadType, // the description of the input variable
		Real64 const CalcDesLoad, // the value from the sizing calculation [W]
		Real64 const UserDesLoad, // the value from the sizing calculation modified by user input [W]
		Real64 const CalcDesFlow, // calculated design air flow rate [m3/s]
		Real64 const UserDesFlow, // user input or modified design air flow rate [m3/s]
		std::string const & DesDayName, // the name of the design day that produced the peak
		std::string const & PeakHrMin, // time stamp of the peak
		Real64 const PeakTemp, // temperature at peak [C]
		Real64 const PeakHumRat, // humidity ratio at peak [kg water/kg dry air]
		Real64 const MinOAVolFlow // zone design minimum outside air flow rate [m3/s]
	);

	void addSQLiteSystemSizingRecord(
		std::string const & SysName, // the name of the system
		std::string const & VarDesc, // the description of the input variable
		Real64 const VarValue // the value from the sizing calculation
	);

	void addSQLiteComponentSizingRecord(
		std::string const & CompType, // the type of the component
		std::string const & CompName, // the name of the component
		std::string const & VarDesc, // the description of the input variable
		Real64 const VarValue // the value from the sizing calculation
	);

	void createSQLiteRoomAirModelTable();

	void createSQLiteMeterDictionaryRecord(
		int const meterReportID,
		int const storeTypeIndex,
		std::string const & indexGroup,
		std::string const & keyedValueString,
		std::string const & variableName,
		int const indexType,
		std::string const & units,
		int const reportingFreq,
		Optional_string_const ScheduleName = _
	);

	void createSQLiteMeterRecord(
		int const recordIndex,
		Real64 const value,
		Optional_int_const reportingInterval = _,
		Optional< Real64 const > minValue = _,
		Optional_int_const minValueDate = _,
		Optional< Real64 const > maxValue = _,
		Optional_int_const maxValueDate = _,
		Optional_int_const minutesPerTimeStep = _
	);

	void createSQLiteDaylightMapTitle(
		int const mapNum,
		std::string const & mapName,
		std::string const & environmentName,
		int const zone,
		std::string const & refPt1,
		std::string const & refPt2,
		Real64 const zCoord
	);

	void createSQLiteDaylightMap(
		int const mapNum,
		int const month,
		int const dayOfMonth,
		int const hourOfDay,
		int const nX,
		FArray1S< Real64 > const x,
		int const nY,
		FArray1S< Real64 > const y,
		FArray2S< Real64 > const illuminance
	);

	void createSQLiteTabularDataRecords(
		FArray2D_string const & body, // row,column
		FArray1D_string const & rowLabels,
		FArray1D_string const & columnLabels,
		std::string const & ReportName,
		std::string const & ReportForString,
		std::string const & TableName
	);

	void createSQLiteSimulationsRecord( int const ID );

	void createSQLiteErrorRecord(
		int const simulationIndex,
		int const errorType,
		std::string const & errorMessage,
		int const cnt
	);

	void updateSQLiteErrorRecord( std::string const & errorMessage );

	void updateSQLiteSimulationRecord(
		bool const completed,
		bool const completedSuccessfully
	);

	void createSQLiteEnvironmentPeriodRecord();

	void sqliteWriteMessage(const std::string & message);

	void createZoneExtendedOutput();

	void initializeIndexes();

private:

	void createSQLiteZoneTable();
	void createSQLiteNominalLightingTable();
	void createSQLiteNominalPeopleTable();
	void createSQLiteNominalElectricEquipmentTable();
	void createSQLiteNominalGasEquipmentTable();
	void createSQLiteNominalSteamEquipmentTable();
	void createSQLiteNominalHotWaterEquipmentTable();
	void createSQLiteNominalOtherEquipmentTable();
	void createSQLiteNominalBaseboardHeatTable();
	void createSQLiteInfiltrationTable();
	void createSQLiteVentilationTable();
	void createSQLiteSurfacesTable();
	void createSQLiteConstructionsTable();
	void createSQLiteMaterialsTable();
	void createSQLiteZoneListTable();
	void createSQLiteZoneGroupTable();
	void createSQLiteSchedulesTable();

	int sqliteExecuteCommand(const std::string & commandBuffer);
	int sqlitePrepareStatement(sqlite3_stmt* & stmt, const std::string & stmtBuffer);

	int sqliteBindText(sqlite3_stmt * stmt, const int stmtInsertLocationIndex, const std::string & textBuffer);
	int sqliteBindInteger(sqlite3_stmt * stmt, const int stmtInsertLocationIndex, const int intToInsert);
	int sqliteBindDouble(sqlite3_stmt * stmt, const int stmtInsertLocationIndex, const double doubleToInsert);
	int sqliteBindNULL(sqlite3_stmt * stmt, const int stmtInsertLocationIndex);
	int sqliteBindLogical(sqlite3_stmt * stmt, const int stmtInsertLocationIndex, const bool valueToInsert);

	int sqliteStepCommand(sqlite3_stmt * stmt);
	int sqliteResetCommand(sqlite3_stmt * stmt);
	int sqliteClearBindings(sqlite3_stmt * stmt);
	int sqliteFinalizeCommand(sqlite3_stmt * stmt);

	int createSQLiteStringTableRecord(std::string const & stringValue,std::string const & stringType);

	static std::string storageType(const int storageTypeIndex);
	static std::string timestepTypeName(const int timestepType);
	static std::string reportingFreqName(const int reportingFreqIndex);

	static void adjustReportingHourAndMinutes(int & hour, int & minutes);
	// Given combinedString, parse out units and description.
	// Example: Given combinedString "Total Energy [GJ]", return "Total Energy"
	// in description and "GJ" in units.
	static void parseUnitsAndDescription(const std::string & combinedString, std::string & units, std::string & description);

	static int logicalToInteger(const bool value);

	void initializeReportVariableDataDictionaryTable();
	void initializeReportVariableDataTables();
	void initializeReportMeterDataDictionaryTable();
	void initializeReportMeterDataTables();
	void initializeTimeIndicesTable();
	void initializeZoneInfoTable();
	void initializeZoneInfoZoneListTable();
	void initializeNominalPeopleTable();
	void initializeNominalLightingTable();
	void initializeNominalElectricEquipmentTable();
	void initializeNominalGasEquipmentTable();
	void initializeNominalSteamEquipmentTable();
	void initializeNominalHotWaterEquipmentTable();
	void initializeNominalOtherEquipmentTable();
	void initializeNominalBaseboardHeatTable();
	void initializeSurfacesTable();
	void initializeConstructionsTables();
	void initializeMaterialsTable();
	void initializeZoneListTable();
	void initializeZoneGroupTable();
	void initializeNominalInfiltrationTable();
	void initializeNominalVentilationTable();
	void initializeZoneSizingTable();
	void initializeSystemSizingTable();
	void initializeComponentSizingTable();
	void initializeRoomAirModelTable();
	void initializeSchedulesTable();
	void initializeDaylightMapTables();
	void initializeViews();
	void initializeSimulationsTable();
	void initializeEnvironmentPeriodsTable();
	void initializeErrorsTable();
	void initializeTabularDataTable();
	void initializeTabularDataView();

	bool m_writeOutputToSQLite;
	bool m_writeTabularDataToSQLite;
	int m_sqlDBTimeIndex;
	std::ofstream m_errorStream;
	sqlite3 * m_db;
	std::string m_dbName;
	sqlite3_stmt * m_reportVariableDataInsertStmt;
	sqlite3_stmt * m_reportVariableExtendedDataInsertStmt;
	sqlite3_stmt * m_timeIndexInsertStmt;
	sqlite3_stmt * m_reportVariableDictionaryInsertStmt;
	sqlite3_stmt * m_zoneInfoInsertStmt;
	sqlite3_stmt * m_zoneInfoZoneListInsertStmt;
	sqlite3_stmt * m_nominalLightingInsertStmt;
	sqlite3_stmt * m_nominalElectricEquipmentInsertStmt;
	sqlite3_stmt * m_nominalGasEquipmentInsertStmt;
	sqlite3_stmt * m_nominalSteamEquipmentInsertStmt;
	sqlite3_stmt * m_nominalHotWaterEquipmentInsertStmt;
	sqlite3_stmt * m_nominalOtherEquipmentInsertStmt;
	sqlite3_stmt * m_nominalBaseboardHeatInsertStmt;
	sqlite3_stmt * m_surfaceInsertStmt;
	sqlite3_stmt * m_constructionInsertStmt;
	sqlite3_stmt * m_constructionLayerInsertStmt;
	sqlite3_stmt * m_materialInsertStmt;
	sqlite3_stmt * m_zoneListInsertStmt;
	sqlite3_stmt * m_zoneGroupInsertStmt;
	sqlite3_stmt * m_infiltrationInsertStmt;
	sqlite3_stmt * m_ventilationInsertStmt;
	sqlite3_stmt * m_nominalPeopleInsertStmt;
	sqlite3_stmt * m_zoneSizingInsertStmt;
	sqlite3_stmt * m_systemSizingInsertStmt;
	sqlite3_stmt * m_componentSizingInsertStmt;
	sqlite3_stmt * m_roomAirModelInsertStmt;
	sqlite3_stmt * m_groundTemperatureInsertStmt;
	sqlite3_stmt * m_weatherFileInsertStmt;
	sqlite3_stmt * m_meterDictionaryInsertStmt;
	sqlite3_stmt * m_reportMeterDataInsertStmt;
	sqlite3_stmt * m_meterExtendedDataInsertStmt;
	sqlite3_stmt * m_scheduleInsertStmt;
	sqlite3_stmt * m_daylightMapTitleInsertStmt;
	sqlite3_stmt * m_daylightMapHorlyTitleInsertStmt;
	sqlite3_stmt * m_daylightMapHorlyDataInsertStmt;
	sqlite3_stmt * m_environmentPeriodInsertStmt;
	sqlite3_stmt * m_simulationsInsertStmt;
	sqlite3_stmt * m_tabularDataInsertStmt;
	sqlite3_stmt * m_stringsInsertStmt;
	sqlite3_stmt * m_stringsLookUpStmt;
	sqlite3_stmt * m_errorInsertStmt;
	sqlite3_stmt * m_errorUpdateStmt;
	sqlite3_stmt * m_simulationUpdateStmt;

	static const int LocalReportEach;      //  Write out each time UpdatedataandLocalReport is called
	static const int LocalReportTimeStep;  //  Write out at 'EndTimeStepFlag'
	static const int LocalReportHourly;    //  Write out at 'EndHourFlag'
	static const int LocalReportDaily;     //  Write out at 'EndDayFlag'
	static const int LocalReportMonthly;   //  Write out at end of month (must be determined)
	static const int LocalReportSim;       //  Write out once per environment 'EndEnvrnFlag'
<<<<<<< HEAD
	static const std::string ReportNameId;         // These should be integers.  Why is this?
	static const std::string ReportForStringId;
	static const std::string TableNameId;
	static const std::string RowNameId;
	static const std::string ColumnNameId;
	static const std::string UnitsId;
=======
	static const int ReportNameId;
	static const int ReportForStringId;
	static const int TableNameId;
	static const int RowNameId;
	static const int ColumnNameId;
	static const int UnitsId;

	class SQLiteData : public SQLiteProcedures
	{
		protected:
			SQLiteData( std::ostream & errorStream, std::shared_ptr<sqlite3> & db );
			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt ) = 0;
	};

	class Schedule : SQLiteData
	{
		public:
			Schedule( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const scheduleNumber, std::string const & scheduleName, 
					std::string const & scheduleType, double const & scheduleMinValue, double const & scheduleMaxValue ) :
				SQLiteData( errorStream, db ),
				number( scheduleNumber ),
				name( scheduleName ),
				type( scheduleType ),
				minValue( scheduleMinValue ),
				maxValue( scheduleMaxValue )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			std::string const & type;
			double const & minValue;
			double const & maxValue;
	};

	class Surface : SQLiteData
	{
		public:
			Surface( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const surfaceNumber, DataSurfaces::SurfaceData const & surfaceData, std::string const & surfaceClass ) :
				SQLiteData( errorStream, db ),
				number( surfaceNumber ),
				name( surfaceData.Name ),
				construction( surfaceData.Construction ),
				surfaceClass( surfaceClass ),
				area( surfaceData.Area ),
				grossArea( surfaceData.GrossArea ),
				perimeter( surfaceData.Perimeter ),
				azimuth( surfaceData.Azimuth ),
				height( surfaceData.Height ),
				reveal( surfaceData.Reveal ),
				shape( surfaceData.Shape ),
				sides( surfaceData.Sides ),
				tilt( surfaceData.Tilt ),
				width( surfaceData.Width ),
				heatTransSurf( surfaceData.HeatTransSurf ),
				baseSurf( surfaceData.BaseSurf ),
				zone( surfaceData.Zone ),
				extBoundCond( surfaceData.ExtBoundCond ),
				extSolar( surfaceData.ExtSolar ),
				extWind( surfaceData.ExtWind )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & construction;
			std::string const & surfaceClass;
			double const & area;
			double const & grossArea;
			double const & perimeter;
			double const & azimuth;
			double const & height;
			double const & reveal;
			int const & shape;
			int const & sides;
			double const & tilt;
			double const & width;
			bool const & heatTransSurf;
			int const & baseSurf;
			int const & zone;
			int const & extBoundCond;
			bool const & extSolar;
			bool const & extWind;
	};

	class Zone : SQLiteData
	{
		public:
			Zone( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const zoneNumber, DataHeatBalance::ZoneData const & zoneData ) :
				SQLiteData( errorStream, db ),
				number( zoneNumber ),
				name( zoneData.Name ),
				relNorth( zoneData.RelNorth ),
				originX( zoneData.OriginX ),
				originY( zoneData.OriginY ),
				originZ( zoneData.OriginZ ),
				centroidX( zoneData.Centroid.x ),
				centroidY( zoneData.Centroid.y ),
				centroidZ( zoneData.Centroid.z ),
				ofType( zoneData.OfType ),
				multiplier( zoneData.Multiplier ),
				listMultiplier( zoneData.ListMultiplier ),
				minimumX( zoneData.MinimumX ),
				maximumX( zoneData.MaximumX ),
				minimumY( zoneData.MinimumY ),
				maximumY( zoneData.MaximumY ),
				minimumZ( zoneData.MinimumZ ),
				maximumZ( zoneData.MaximumZ ),
				ceilingHeight( zoneData.CeilingHeight ),
				volume( zoneData.Volume ),
				insideConvectionAlgo( zoneData.InsideConvectionAlgo ),
				outsideConvectionAlgo( zoneData.OutsideConvectionAlgo ),
				floorArea( zoneData.FloorArea ),
				extGrossWallArea( zoneData.ExtGrossWallArea ),
				extNetWallArea( zoneData.ExtNetWallArea ),
				extWindowArea( zoneData.ExtWindowArea ),
				isPartOfTotalArea( zoneData.isPartOfTotalArea )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			double const & relNorth;
			double const & originX;
			double const & originY;
			double const & originZ;
			double const & centroidX;
			double const & centroidY;
			double const & centroidZ;
			int const & ofType;
			int const & multiplier;
			int const & listMultiplier;
			double const & minimumX;
			double const & maximumX;
			double const & minimumY;
			double const & maximumY;
			double const & minimumZ;
			double const & maximumZ;
			double const & ceilingHeight;
			double const & volume;
			int const & insideConvectionAlgo;
			int const & outsideConvectionAlgo;
			double const & floorArea;
			double const & extGrossWallArea;
			double const & extNetWallArea;
			double const & extWindowArea;
			bool const & isPartOfTotalArea;
	};

	class ZoneList : SQLiteData
	{
		public:
			ZoneList( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const zoneListNumber, DataHeatBalance::ZoneListData const & zoneListData ) :
				SQLiteData( errorStream, db ),
				number( zoneListNumber ),
				name( zoneListData.Name ),
				zones( zoneListData.Zone )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );
			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt, sqlite3_stmt * subInsertStmt );

		private:
			int const number;
			std::string const & name;
			FArray1D_int const & zones;
	};

	class ZoneGroup : SQLiteData
	{
		public:
			ZoneGroup( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const zoneGroupNumber, DataHeatBalance::ZoneGroupData const & zoneGroupData ) :
				SQLiteData( errorStream, db ),
				number( zoneGroupNumber ),
				name( zoneGroupData.Name ),
				zoneList( zoneGroupData.ZoneList ),
				multiplier( zoneGroupData.Multiplier )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zoneList;
			int const & multiplier;
	};

	class Material : SQLiteData
	{
		public:
			Material( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const materialNumber, DataHeatBalance::MaterialProperties const & materialData ) :
				SQLiteData( errorStream, db ),
				number( materialNumber ),
				name( materialData.Name ),
				group( materialData.Group  ),
				roughness( materialData.Roughness ),
				conductivity( materialData.Conductivity ),
				density( materialData.Density ),
				isoMoistCap( materialData.IsoMoistCap ),
				porosity( materialData.Porosity ),
				resistance( materialData.Resistance ),
				rOnly( materialData.ROnly ),
				specHeat( materialData.SpecHeat ),
				thermGradCoef( materialData.ThermGradCoef ),
				thickness( materialData.Thickness ),
				vaporDiffus( materialData.VaporDiffus )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & group;
			int const & roughness;
			double const & conductivity;
			double const & density;
			double const & isoMoistCap;
			double const & porosity;
			double const & resistance;
			bool const & rOnly;
			double const & specHeat;
			double const & thermGradCoef;
			double const & thickness;
			double const & vaporDiffus;
	};

	class Construction : SQLiteData
	{
		public:
			Construction( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const constructionNumber, DataHeatBalance::ConstructionData const & constructionData, double const & constructionUValue ) :
				SQLiteData( errorStream, db ),
				number( constructionNumber ),
				name( constructionData.Name ),
				totLayers( constructionData.TotLayers ),
				totSolidLayers( constructionData.TotSolidLayers ),
				totGlassLayers( constructionData.TotGlassLayers ),
				insideAbsorpVis( constructionData.InsideAbsorpVis ),
				outsideAbsorpVis( constructionData.OutsideAbsorpVis ),
				insideAbsorpSolar( constructionData.InsideAbsorpSolar ),
				outsideAbsorpSolar( constructionData.OutsideAbsorpSolar ),
				insideAbsorpThermal( constructionData.InsideAbsorpThermal ),
				outsideAbsorpThermal( constructionData.OutsideAbsorpThermal ),
				outsideRoughness( constructionData.OutsideRoughness ),
				typeIsWindow( constructionData.TypeIsWindow ),
				uValue( constructionUValue )
			{
				for(int layerNum = 1; layerNum <= constructionData.TotLayers; ++layerNum) {
					constructionLayers.push_back(
						std::unique_ptr<Construction::ConstructionLayer>(new ConstructionLayer(m_errorStream, m_db, number, layerNum, constructionData.LayerPoint(layerNum)))
					);
				}
			}

			// only inserts construction
			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );
			// inserts construction and construction layers
			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt, sqlite3_stmt * subInsertStmt );

		private:
			int const number;
			std::string const & name;
			int const & totLayers;
			int const & totSolidLayers;
			int const & totGlassLayers;
			double const & insideAbsorpVis;
			double const & outsideAbsorpVis;
			double const & insideAbsorpSolar;
			double const & outsideAbsorpSolar;
			double const & insideAbsorpThermal;
			double const & outsideAbsorpThermal;
			int const & outsideRoughness;
			bool const & typeIsWindow;
			double const & uValue;

			class ConstructionLayer : SQLiteData
			{
				public:
					ConstructionLayer( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const & constructNumber, int const layerNumber, int const & layerPoint ) :
						SQLiteData( errorStream, db ),
						constructNumber( constructNumber ),
						layerNumber( layerNumber ),
						layerPoint( layerPoint )
					{}

					virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

				private:
					int const & constructNumber;
					int const layerNumber;
					int const & layerPoint;
			};

			std::vector< std::unique_ptr<Construction::ConstructionLayer> > constructionLayers;
	};

	class NominalLighting : SQLiteData
	{
		public:
			NominalLighting( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalLightingNumber, DataHeatBalance::LightsData const & nominalLightingData ) :
				SQLiteData( errorStream, db ),
				number( nominalLightingNumber ),
				name( nominalLightingData.Name ),
				zonePtr( nominalLightingData.ZonePtr ),
				schedulePtr( nominalLightingData.SchedPtr ),
				designLevel( nominalLightingData.DesignLevel ),
				fractionReturnAir( nominalLightingData.FractionReturnAir ),
				fractionRadiant( nominalLightingData.FractionRadiant ),
				fractionShortWave( nominalLightingData.FractionShortWave ),
				fractionReplaceable( nominalLightingData.FractionReplaceable ),
				fractionConvected( nominalLightingData.FractionConvected ),
				endUseSubcategory( nominalLightingData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedulePtr;
			double const & designLevel;
			double const & fractionReturnAir;
			double const & fractionRadiant;
			double const & fractionShortWave;
			double const & fractionReplaceable;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class NominalPeople : SQLiteData
	{
		public:
			NominalPeople( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalPeopleNumber, DataHeatBalance::PeopleData const & nominalPeopleData ) :
				SQLiteData( errorStream, db ),
				number( nominalPeopleNumber ),
				name( nominalPeopleData.Name ),
				zonePtr( nominalPeopleData.ZonePtr ),
				numberOfPeople( nominalPeopleData.NumberOfPeople ),
				numberOfPeoplePtr( nominalPeopleData.NumberOfPeoplePtr ),
				activityLevelPtr( nominalPeopleData.ActivityLevelPtr ),
				fractionRadiant( nominalPeopleData.FractionRadiant ),
				fractionConvected( nominalPeopleData.FractionConvected ),
				workEffPtr( nominalPeopleData.WorkEffPtr ),
				clothingPtr( nominalPeopleData.ClothingPtr ),
				airVelocityPtr( nominalPeopleData.AirVelocityPtr ),
				fanger( nominalPeopleData.Fanger ),
				pierce( nominalPeopleData.Pierce ),
				ksu( nominalPeopleData.KSU ),
				mrtCalcType( nominalPeopleData.MRTCalcType ),
				surfacePtr( nominalPeopleData.SurfacePtr ),
				angleFactorListName( nominalPeopleData.AngleFactorListName ),
				angleFactorListPtr( nominalPeopleData.AngleFactorListPtr ),
				userSpecSensFrac( nominalPeopleData.UserSpecSensFrac ),
				show55Warning( nominalPeopleData.Show55Warning )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			double const & numberOfPeople;
			int const & numberOfPeoplePtr;
			int const & activityLevelPtr;
			double const & fractionRadiant;
			double const & fractionConvected;
			int const & workEffPtr;
			int const & clothingPtr;
			int const & airVelocityPtr;
			bool const & fanger;
			bool const & pierce;
			bool const & ksu;
			int const & mrtCalcType;
			int const & surfacePtr;
			std::string const & angleFactorListName;
			int const &  angleFactorListPtr;
			double const & userSpecSensFrac;
			bool const & show55Warning;
	};

	class NominalElectricEquipment : SQLiteData
	{
		public:
			NominalElectricEquipment( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalElectricEquipmentNumber, DataHeatBalance::ZoneEquipData const & nominalElectricEquipmentData ) :
				SQLiteData( errorStream, db ),
				number( nominalElectricEquipmentNumber ),
				name( nominalElectricEquipmentData.Name ),
				zonePtr( nominalElectricEquipmentData.ZonePtr ),
				schedulePtr( nominalElectricEquipmentData.SchedPtr ),
				designLevel( nominalElectricEquipmentData.DesignLevel ),
				fractionLatent( nominalElectricEquipmentData.FractionLatent ),
				fractionRadiant( nominalElectricEquipmentData.FractionRadiant ),
				fractionLost( nominalElectricEquipmentData.FractionLost ),
				fractionConvected( nominalElectricEquipmentData.FractionConvected ),
				endUseSubcategory( nominalElectricEquipmentData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedulePtr;
			double const & designLevel;
			double const & fractionLatent;
			double const & fractionRadiant;
			double const & fractionLost;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class NominalGasEquipment : SQLiteData
	{
		public:
			NominalGasEquipment( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalGasEquipmentNumber, DataHeatBalance::ZoneEquipData const & nominalGasEquipmentData ) :
				SQLiteData( errorStream, db ),
				number( nominalGasEquipmentNumber ),
				name( nominalGasEquipmentData.Name ),
				zonePtr( nominalGasEquipmentData.ZonePtr ),
				schedulePtr( nominalGasEquipmentData.SchedPtr ),
				designLevel( nominalGasEquipmentData.DesignLevel ),
				fractionLatent( nominalGasEquipmentData.FractionLatent ),
				fractionRadiant( nominalGasEquipmentData.FractionRadiant ),
				fractionLost( nominalGasEquipmentData.FractionLost ),
				fractionConvected( nominalGasEquipmentData.FractionConvected ),
				endUseSubcategory( nominalGasEquipmentData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedulePtr;
			double const & designLevel;
			double const & fractionLatent;
			double const & fractionRadiant;
			double const & fractionLost;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class NominalSteamEquipment : SQLiteData
	{
		public:
			NominalSteamEquipment( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalSteamEquipmentNumber, DataHeatBalance::ZoneEquipData const & nominalSteamEquipmentData ) :
				SQLiteData( errorStream, db ),
				number( nominalSteamEquipmentNumber ),
				name( nominalSteamEquipmentData.Name ),
				zonePtr( nominalSteamEquipmentData.ZonePtr ),
				schedulePtr( nominalSteamEquipmentData.SchedPtr ),
				designLevel( nominalSteamEquipmentData.DesignLevel ),
				fractionLatent( nominalSteamEquipmentData.FractionLatent ),
				fractionRadiant( nominalSteamEquipmentData.FractionRadiant ),
				fractionLost( nominalSteamEquipmentData.FractionLost ),
				fractionConvected( nominalSteamEquipmentData.FractionConvected ),
				endUseSubcategory( nominalSteamEquipmentData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedulePtr;
			double const & designLevel;
			double const & fractionLatent;
			double const & fractionRadiant;
			double const & fractionLost;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class NominalHotWaterEquipment : SQLiteData
	{
		public:
			NominalHotWaterEquipment( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalHotWaterEquipmentNumber, DataHeatBalance::ZoneEquipData const & nominalHotWaterEquipmentData ) :
				SQLiteData( errorStream, db ),
				number( nominalHotWaterEquipmentNumber ),
				name( nominalHotWaterEquipmentData.Name ),
				zonePtr( nominalHotWaterEquipmentData.ZonePtr ),
				schedulePtr( nominalHotWaterEquipmentData.SchedPtr ),
				designLevel( nominalHotWaterEquipmentData.DesignLevel ),
				fractionLatent( nominalHotWaterEquipmentData.FractionLatent ),
				fractionRadiant( nominalHotWaterEquipmentData.FractionRadiant ),
				fractionLost( nominalHotWaterEquipmentData.FractionLost ),
				fractionConvected( nominalHotWaterEquipmentData.FractionConvected ),
				endUseSubcategory( nominalHotWaterEquipmentData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedulePtr;
			double const & designLevel;
			double const & fractionLatent;
			double const & fractionRadiant;
			double const & fractionLost;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class NominalOtherEquipment : SQLiteData
	{
		public:
			NominalOtherEquipment( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalOtherEquipmentNumber, DataHeatBalance::ZoneEquipData const & nominalOtherEquipmentData ) :
				SQLiteData( errorStream, db ),
				number( nominalOtherEquipmentNumber ),
				name( nominalOtherEquipmentData.Name ),
				zonePtr( nominalOtherEquipmentData.ZonePtr ),
				schedulePtr( nominalOtherEquipmentData.SchedPtr ),
				designLevel( nominalOtherEquipmentData.DesignLevel ),
				fractionLatent( nominalOtherEquipmentData.FractionLatent ),
				fractionRadiant( nominalOtherEquipmentData.FractionRadiant ),
				fractionLost( nominalOtherEquipmentData.FractionLost ),
				fractionConvected( nominalOtherEquipmentData.FractionConvected ),
				endUseSubcategory( nominalOtherEquipmentData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedulePtr;
			double const & designLevel;
			double const & fractionLatent;
			double const & fractionRadiant;
			double const & fractionLost;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class NominalBaseboardHeat : SQLiteData
	{
		public:
			NominalBaseboardHeat( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const nominalBaseboardHeatNumber, DataHeatBalance::BBHeatData const & nominalBaseboardHeatData ) :
				SQLiteData( errorStream, db ),
				number( nominalBaseboardHeatNumber ),
				name( nominalBaseboardHeatData.Name ),
				zonePtr( nominalBaseboardHeatData.ZonePtr ),
				schedPtr( nominalBaseboardHeatData.SchedPtr ),
				capatLowTemperature( nominalBaseboardHeatData.CapatLowTemperature ),
				lowTemperature( nominalBaseboardHeatData.LowTemperature ),
				capatHighTemperature( nominalBaseboardHeatData.CapatHighTemperature ),
				highTemperature( nominalBaseboardHeatData.HighTemperature ),
				fractionRadiant( nominalBaseboardHeatData.FractionRadiant ),
				fractionConvected( nominalBaseboardHeatData.FractionConvected ),
				endUseSubcategory( nominalBaseboardHeatData.EndUseSubcategory )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedPtr;
			double const & capatLowTemperature;
			double const & lowTemperature;
			double const & capatHighTemperature;
			double const & highTemperature;
			double const & fractionRadiant;
			double const & fractionConvected;
			std::string const & endUseSubcategory;
	};

	class Infiltration : SQLiteData
	{
		public:
			Infiltration( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const infiltrationNumber, DataHeatBalance::InfiltrationData const & infiltrationData ) :
				SQLiteData( errorStream, db ),
				number( infiltrationNumber ),
				name( infiltrationData.Name ),
				zonePtr( infiltrationData.ZonePtr ),
				schedPtr( infiltrationData.SchedPtr ),
				designLevel( infiltrationData.DesignLevel )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedPtr;
			double const & designLevel;
	};

	class Ventilation : SQLiteData
	{
		public:
			Ventilation( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const ventilationNumber, DataHeatBalance::VentilationData const & ventilationData ) :
				SQLiteData( errorStream, db ),
				number( ventilationNumber ),
				name( ventilationData.Name ),
				zonePtr( ventilationData.ZonePtr ),
				schedPtr( ventilationData.SchedPtr ),
				designLevel( ventilationData.DesignLevel )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & name;
			int const & zonePtr;
			int const & schedPtr;
			double const & designLevel;
	};

	class RoomAirModel : SQLiteData
	{
		public:
			RoomAirModel( std::ostream & errorStream, std::shared_ptr<sqlite3> & db, int const roomAirModelNumber, DataRoomAirModel::AirModelData const & roomAirModelData ) :
				SQLiteData( errorStream, db ),
				number( roomAirModelNumber ),
				airModelName( roomAirModelData.AirModelName ),
				airModelType( roomAirModelData.AirModelType ),
				tempCoupleScheme( roomAirModelData.TempCoupleScheme ),
				simAirModel( roomAirModelData.SimAirModel )
			{}

			virtual bool insertIntoSQLite( sqlite3_stmt * insertStmt );

		private:
			int const number;
			std::string const & airModelName;
			int const & airModelType;
			int const & tempCoupleScheme;
			bool const & simAirModel;
	};

	std::vector< std::unique_ptr<SQLite::Zone> > zones;
	std::vector< std::unique_ptr<SQLite::ZoneList> > zoneLists;
	std::vector< std::unique_ptr<SQLite::ZoneGroup> > zoneGroups;
	std::vector< std::unique_ptr<SQLite::Schedule> > schedules;
	std::vector< std::unique_ptr<SQLite::Surface> > surfaces;
	std::vector< std::unique_ptr<SQLite::Material> > materials;
	std::vector< std::unique_ptr<SQLite::Construction> > constructions;
	std::vector< std::unique_ptr<SQLite::NominalLighting> > nominalLightings;
	std::vector< std::unique_ptr<SQLite::NominalPeople> > nominalPeoples;
	std::vector< std::unique_ptr<SQLite::NominalElectricEquipment> > nominalElectricEquipments;
	std::vector< std::unique_ptr<SQLite::NominalGasEquipment> > nominalGasEquipments;
	std::vector< std::unique_ptr<SQLite::NominalSteamEquipment> > nominalSteamEquipments;
	std::vector< std::unique_ptr<SQLite::NominalHotWaterEquipment> > nominalHotWaterEquipments;
	std::vector< std::unique_ptr<SQLite::NominalOtherEquipment> > nominalOtherEquipments;
	std::vector< std::unique_ptr<SQLite::NominalBaseboardHeat> > nominalBaseboardHeats;
	std::vector< std::unique_ptr<SQLite::Infiltration> > infiltrations;
	std::vector< std::unique_ptr<SQLite::Ventilation> > ventilations;
	std::vector< std::unique_ptr<SQLite::RoomAirModel> > roomAirModels;
>>>>>>> 3e6eb0a... Add zoneList and zoneGroup, add more integration
};

extern std::unique_ptr<SQLite> sqlite;

} // EnergyPlus

#endif
