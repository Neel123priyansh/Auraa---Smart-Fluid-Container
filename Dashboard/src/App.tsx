import { useEffect, useState } from "react";
import { Line } from "react-chartjs-2";
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
} from "chart.js";

// Register chart.js components
ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement, Title, Tooltip, Legend);

const App = () => {
  const [bpmData, setBpmData] = useState<number[]>([]);
  const [spO2Data, setSpO2Data] = useState<number[]>([]);
  const [labels, setLabels] = useState<string[]>([]);

  useEffect(() => {
    const fetchVitals = async () => {
      try {
        const response = await fetch("http://localhost:3000/api/vitals"); // Backend API
        if (!response.ok) {
          throw new Error("Network response was not ok");
        }
        const data = await response.json();

        // Process fetched data
        const newBpmData = data.map((vital: any) => vital.bpm);
        const newSpO2Data = data.map((vital: any) => vital.spo2);
        const newLabels = data.map((vital: any) => new Date(vital.timestamp).toLocaleTimeString());

        setBpmData(newBpmData);
        setSpO2Data(newSpO2Data);
        setLabels(newLabels);
      } catch (error) {
        console.error("Fetch error:", error);
      }
    };

    fetchVitals(); // Fetch data on mount
    const interval = setInterval(fetchVitals, 5000); // Fetch data every 5 seconds
    return () => clearInterval(interval); // Cleanup interval on unmount
  }, []);

  const bpmChartData = {
    labels,
    datasets: [
      {
        label: "Heart Rate (BPM)",
        data: bpmData,
        borderColor: "rgba(255, 99, 132, 1)",
        backgroundColor: "rgba(255, 99, 132, 0.2)",
        tension: 0.4,
      },
    ],
  };

  const spO2ChartData = {
    labels,
    datasets: [
      {
        label: "SpO2 (%)",
        data: spO2Data,
        borderColor: "rgba(54, 162, 235, 1)",
        backgroundColor: "rgba(54, 162, 235, 0.2)",
        tension: 0.4,
      },
    ],
  };

  return (
    <div className="flex flex-col items-center justify-center h-screen bg-gray-100">
      <h1 className="text-2xl font-bold mb-4">Health Dashboard</h1>
      <div className="flex flex-wrap space-x-4">
        {/* BPM Graph */}
        <div className="bg-white p-4 rounded-lg shadow-md w-full md:w-1/2">
          <h2 className="text-lg font-semibold mb-2">Heart Rate (BPM)</h2>
          <div className="h-64">
            <Line data={bpmChartData} options={{ responsive: true, maintainAspectRatio: false }} />
          </div>
        </div>
        {/* SpO2 Graph */}
        <div className="bg-white p-4 rounded-lg shadow-md w-full md:w-1/2">
          <h2 className="text-lg font-semibold mb-2">SpO2 (%)</h2>
          <div className="h-64">
            <Line data={spO2ChartData} options={{ responsive: true, maintainAspectRatio: false }} />
          </div>
        </div>
      </div>
    </div>
  );
};

export default App;
