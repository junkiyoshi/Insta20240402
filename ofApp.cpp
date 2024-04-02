#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofNoFill();
	ofEnableDepthTest();

	ofColor color;
	this->number_of_sphere = 70;
	while (this->box_list.size() < this->number_of_sphere) {

		auto tmp_location = this->make_point(280, ofRandom(0, 50), ofRandom(360), ofRandom(360));
		auto radius = this->box_list.size() < 50 ? ofRandom(20, 60) : ofRandom(10, 20);

		bool flag = true;
		for (int i = 0; i < this->box_list.size(); i++) {

			if (glm::distance(tmp_location, get<1>(this->box_list[i])) < get<2>(this->box_list[i]) + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			color.setHsb(ofRandom(255), 255, 200);
			this->box_list.push_back(make_tuple(color, tmp_location, radius));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(ofGetFrameNum() * 0.77);

	for (int i = 0; i < this->box_list.size(); i++) {

		auto color = get<0>(this->box_list[i]);
		auto location = get<1>(this->box_list[i]);
		auto radius = get<2>(this->box_list[i]);

		ofPushMatrix();
		ofTranslate(location);

		for (int k = 0; k < 8; k++) {

			auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
			color.setHsb(int(color.getHue() + 32) % 255, 255, 255);

			for (int m = 0; m < 200; m++) {
				
				ofSetColor(color, ofMap(m, 0, 200, 32, 255));

				auto vertex = glm::vec3(
					ofMap(ofNoise(noise_seed.x, m * 0.005 + ofGetFrameNum() * 0.085), 0, 1, -radius * 0.85, radius * 0.85),
					ofMap(ofNoise(noise_seed.y, m * 0.005 + ofGetFrameNum() * 0.085), 0, 1, -radius * 0.85, radius * 0.85),
					ofMap(ofNoise(noise_seed.z, m * 0.005 + ofGetFrameNum() * 0.085), 0, 1, -radius * 0.85, radius * 0.85)
				);

				auto next = glm::vec3(
					ofMap(ofNoise(noise_seed.x, (m + 1) * 0.005 + ofGetFrameNum() * 0.085), 0, 1, -radius * 0.85, radius * 0.85),
					ofMap(ofNoise(noise_seed.y, (m + 1) * 0.005 + ofGetFrameNum() * 0.085), 0, 1, -radius * 0.85, radius * 0.85),
					ofMap(ofNoise(noise_seed.z, (m + 1) * 0.005 + ofGetFrameNum() * 0.085), 0, 1, -radius * 0.85, radius * 0.85)
				);

				vertex = glm::normalize(vertex) * radius * 0.9;
				next = glm::normalize(next) * radius * 0.9;

				ofDrawLine(vertex, next);
			}
		}

		ofPopMatrix();
	}

	this->cam.end();

	/*
	int start = 250;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}