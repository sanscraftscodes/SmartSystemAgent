import sounddevice as sd
import speech_recognition as sr
import numpy as np

# --- SmartSysAgent command execution functions ---
def set_volume(value): print(f"[Volume] Set to {value}%")
def get_volume(): print("[Volume] Current volume: 46%")
def show_battery(): print("[Battery] 42% (Charging)")
def set_brightness(value): print(f"[Brightness] Set to {value}%")
def play_music(path): print(f"[MusicPlayer] Playing: {path}")
def stop_music(): print("[MusicPlayer] Stopped")
def open_app(app): print(f"[AppLauncher] Opening: {app}")
def open_pdf(file): print(f"[AppLauncher] Opening PDF: {file}")
def speak(text): print(f"[VoiceAssistant] Speaking: {text}")
def set_wifi(state): print(f"[WiFi] Set {state}")
def set_bluetooth(state): print(f"[Bluetooth] Set {state}")

# --- Command mapping ---
COMMAND_MAPPING = {
    "setvolume30": lambda: set_volume(30),
    "setvolume50": lambda: set_volume(50),
    "getvolume": lambda: get_volume(),
    "showbattery": lambda: show_battery(),
    "setbrightness60": lambda: set_brightness(60),
    "setbrightness80": lambda: set_brightness(80),
    "playmusic": lambda: play_music("C:\\path\\song.wav"),
    "stopmusic": lambda: stop_music(),
    "openapp": lambda: open_app("VisualStudio.exe"),
    "openpdf": lambda: open_pdf("report.pdf"),
    "speak": lambda: speak("Hello team"),
    "setwifion": lambda: set_wifi("On"),
    "setwifioff": lambda: set_wifi("Off"),
    "setbluetoothon": lambda: set_bluetooth("On"),
    "setbluetoothoff": lambda: set_bluetooth("Off"),
}

# --- Microphone settings ---
MIC_INDEX = 1
SAMPLE_RATE = 16000
DURATION = 5  # seconds

# --- Listening function (your working version) ---
def listen_once():
    r = sr.Recognizer()
    print("[Voice] Listening...")
    audio_data = sd.rec(int(DURATION * SAMPLE_RATE), samplerate=SAMPLE_RATE, channels=1, dtype='int16', device=MIC_INDEX)
    sd.wait()
    audio = sr.AudioData(audio_data.tobytes(), SAMPLE_RATE, 2)

    try:
        text = r.recognize_google(audio)
        print("[Voice] You said:", text)
        text = remove_repetition(text)
        return text
    except sr.UnknownValueError:
        print("[Voice] Could not understand.")
        return ""
    except sr.RequestError:
        print("[Voice] Speech service error.")
        return ""

# --- Remove repeated halves ---
def remove_repetition(text):
    half = len(text) // 2
    if text[:half].strip().lower() == text[half:].strip().lower():
        return text[:half].strip()
    return text.strip()

# --- Execute the command ---
def execute_command(text):
    normalized = text.replace(" ", "").lower()
    action = COMMAND_MAPPING.get(normalized)
    if action:
        action()  # Run the actual command
        print(f"[Executed] {normalized}")
    else:
        print("[Core] Unknown command:", text)

# --- Wrapper for single-use voice command ---
voice_enabled = False  # <-- add at the top
def voice_on():
    global voice_enabled
    if voice_enabled:
        print("[Voice] Already enabled.")
        return
    voice_enabled = True
    print("[Voice] Listening mode activated.")
    command = listen_once()
    if command:
        execute_command(command)
    voice_enabled = False  # automatically disable after listening
    print("[Voice] Voice mode disabled.")

def voice_off():
    global voice_enabled
    if voice_enabled:
        voice_enabled = False
        print("[Voice] Voice mode disabled.")
    else:
        print("[Voice] Voice mode is not active.")


# --- Main ---
if __name__ == "__main__":
    voice_on()