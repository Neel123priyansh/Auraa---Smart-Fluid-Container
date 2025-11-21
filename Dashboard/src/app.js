import express from 'express';
import { connect, Schema, model } from 'mongoose';
import cors from 'cors';

const app = express();
const PORT = 3000;

// MongoDB connection
const MONGO_URI = 'mongodb://localhost:27017/oximeter';
connect(MONGO_URI, { useNewUrlParser: true, useUnifiedTopology: true })
    .then(() => console.log('Connected to MongoDB'))
    .catch(err => console.error('MongoDB connection error:', err));

// Define a schema for vitals data
const vitalsSchema = new Schema({
    spo2: Number,
    bpm: Number,
    timestamp: { type: Date, default: Date.now }
});

// Create a model
const Vitals = model('Vitals', vitalsSchema);

// Middleware
app.use(cors());
app.use(express.json()); // Use the built-in JSON parser in Express

// API endpoint to receive vitals data
app.post('/api/vitals', async (req, res) => {
    try {
        const { spo2, bpm } = req.body;

        // Validate data
        if (!spo2 || !bpm) {
            return res.status(400).json({ error: 'Missing data' });
        }

        // Save data to MongoDB
        const vitals = new Vitals({ spo2, bpm });
        await vitals.save();

        res.status(201).json({ message: 'Vitals data saved successfully' });
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Internal server error' });
    }
});

app.get('/api/vitals', async (req, res) => {
    try {
        // Fetch the most recent 10 records
        const vitals = await Vitals.find().sort({ timestamp: -1 }).limit(10);
        res.status(200).json(vitals);
    } catch (error) {
        console.error("Error fetching vitals:", error);
        res.status(500).json({ error: "Internal server error" });
    }
});


// Start the server
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
