/*
 Navicat Premium Data Transfer

 Source Server         : 192.168.1.9
 Source Server Type    : MySQL
 Source Server Version : 80025
 Source Host           : 192.168.1.9:3306
 Source Schema         : XEngine

 Target Server Type    : MySQL
 Target Server Version : 80025
 File Encoding         : 65001

 Date: 17/06/2021 17:19:26
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for XStorage_Count
-- ----------------------------
DROP TABLE IF EXISTS `XStorage_Count`;
CREATE TABLE `XStorage_Count`  (
  `ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `FileCount` bigint NULL DEFAULT NULL COMMENT '文件总个数',
  `FileSize` bigint NULL DEFAULT NULL COMMENT '文件总大小',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Table structure for XStorage_User
-- ----------------------------
DROP TABLE IF EXISTS `XStorage_User`;
CREATE TABLE `XStorage_User`  (
  `ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `UserName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '用户名',
  `UserPass` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '密码',
  `FileCount` bigint NOT NULL COMMENT '文件个数',
  `FileSize` bigint NOT NULL COMMENT '占用大小',
  `PhoneNumber` bigint NOT NULL COMMENT '电话号码',
  `IDNumber` bigint NOT NULL COMMENT '身份证号码',
  `PermissionLeave` tinyint NOT NULL COMMENT '权限级别',
  `Status` tinyint NULL DEFAULT NULL COMMENT '状态',
  `xhToken` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '用户临时句柄',
  `EMailAddr` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '电子邮件地址',
  `IPAddr` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '最后登录IP地址',
  `CreateTime` datetime NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;
